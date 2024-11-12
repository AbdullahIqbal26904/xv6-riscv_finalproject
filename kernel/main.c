#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "stat.h"
volatile static int started = 0;
#include "sha256.h"
#include "syscall.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
//here i have done performance analysis
#define NUM_RUNS 100
#define NUM_STRINGS 20
//extern int k_sha256(char *data, int len); // SHA-256 function for user space
uint64
gettime(void)
{	
    return r_time();  // Read the real-time clock ticks
}
// Function to initialize strings of varying lengths
void generate_string(char *str, int len) {
    for (int i = 0; i < len; i++) {
        str[i] = 'a' + (i % 26); // Fill with repetitive characters ('a' to 'z')
    }
    str[len] = '\0'; // Null-terminate the string
}
// function to print the hashed value
void print_hash_kernelspace(uint8_t *hash) {
    const char *hex_digits = "0123456789abcdef";
    char hash_string[65];
    int i;

    for (i = 0; i < 32; i++) {
        hash_string[i * 2] = hex_digits[(hash[i] >> 4) & 0xF];
        hash_string[i * 2 + 1] = hex_digits[hash[i] & 0xF];
    }

    hash_string[64] = '\0';
    printf("%s\n", hash_string);
}
// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
     // Insert the code to compute and print the SHA-256 hash here
        //const char *input = "Hello, XV6 Kernel!";
        //uint8_t *hash;  // Pointer to store the resulting hash

        // Calculate the SHA-256 hash of the input string
        //hash = sha256((const uint8_t *)input, strlen(input));

        // Print the SHA-256 hash
        //printf("\nSHA-256 hash of '%s':\n", input);
        //print_hash_kernelspace(hash);  // Use the print_hash_kernelspace function to print the hash

    //performance analysis code is here
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
            uint64 start = gettime();
            sha256((uint8_t *)data, len);  // Call the SHA-256 function without assigning the result
            uint64 end = gettime();
            times[i] = end - start;
            sum += times[i];
        }

        // Calculate and print the average time for the current string length
        uint64 avg_time = sum / NUM_RUNS;
        printf("String length %d: Average execution time: %ld ticks\n", len, avg_time);
    }
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
