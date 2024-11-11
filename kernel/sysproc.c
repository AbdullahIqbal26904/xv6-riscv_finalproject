#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
// implementation of sha256 system call
#include "sha256.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
// function to print the hashed value
void print_hash(uint8_t *hash) {
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

//int k_sha256(char* data, int length);
//handler function for calling sha256
uint64
sys_k_sha256(void) {
    uint64 userdata;
    int length;
    
    argaddr(0, &userdata);
    argint(1, &length);
    
    uint64 kdata[length+1];
    
    //copy data from userspace to kernel space
    either_copyin(kdata, 1, userdata, length+1);
    printf("%s\n", ((uint8_t *)kdata));
    
    
    uint8_t* hash = sha256((uint8_t *)kdata, length);  // Call the SHA-256 function
    
    // Print the hash to the console for verification (optional)
    print_hash(hash);
    printf("\n");

    return 0;  // Return the pointer to the hash
}
//handler function for implementation of wrapper function for getTime
uint64
sys_getTime(void)
{	
    return r_time();  // Read the real-time clock ticks
}
//handler function to get number of system call made
uint64
sys_get_syscall_count(void)
{
  struct proc *p = myproc();
  return p->syscall_count;  // Return the syscall count for the current process
}
uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
