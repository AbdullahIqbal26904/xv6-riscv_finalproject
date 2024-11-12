#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_RUNS 100
#define NUM_STRINGS 20

//extern int k_sha256(char *data, int len); // SHA-256 function for user space

// Function to initialize strings of varying lengths
void generate_string(char *str, int len) {
    for (int i = 0; i < len; i++) {
        str[i] = 'a' + (i % 26); // Fill with repetitive characters ('a' to 'z')
    }
    str[len] = '\0'; // Null-terminate the string
}

int main(int argc, char *argv[]) {
    uint64 times[NUM_RUNS];
    int string_lengths[NUM_STRINGS] = {50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 
                                       250, 270, 290, 310, 330, 350, 370, 390, 410, 420};

    for (int s = 0; s < NUM_STRINGS; s++) {
        int len = string_lengths[s];
        char data[len + 1];
        generate_string(data, len);

        // Run hashing NUM_RUNS times for each string length to get average timing
        uint64 sum = 0;
        for (int i = 0; i < NUM_RUNS; i++) {
            uint64 start = getTime();
            k_sha256(data, len);  // Call the SHA-256 function without assigning the result
            uint64 end = getTime();
            times[i] = end - start;
            sum += times[i];
        }

        // Calculate and print the average time for the current string length
        uint64 avg_time = sum / NUM_RUNS;
        printf("String length %d: Average execution time: %ld ticks\n", len, avg_time);
    }

    exit(0);
}

