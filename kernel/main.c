#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;
#include "sha256.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
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
        const char *input = "Hello, XV6 Kernel!";
        uint8_t *hash;  // Pointer to store the resulting hash

        // Calculate the SHA-256 hash of the input string
        hash = sha256((const uint8_t *)input, strlen(input));

        // Print the SHA-256 hash
        printf("\nSHA-256 hash of '%s':\n", input);
        print_hash_kernelspace(hash);  // Use the print_hash_kernelspace function to print the hash

    
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
