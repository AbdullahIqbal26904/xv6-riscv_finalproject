#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Assuming k_sha256 is defined and accessible from here; otherwise, replace with your SHA-256 function.
extern int k_sha256(char *data, int len);

int main(int argc, char *argv[]) {
    uint64 start;
    uint64 end;
    char data[5] = "hello";  // Sample input data for SHA-256

    // Get the start time
    start = getTime();

    // Call the SHA-256 hashing function (or any function you want to measure)
    int final = k_sha256(data, 5);

    // Get the end time
    end = getTime();

    // Calculate and print the duration
    printf("Execution time for SHA-256 in ticks: %ld\n", end - start);
    printf("Value is encrypted, final result: %d\n", final);  // Optional, based on your k_sha256 implementation

    exit(0);
}

