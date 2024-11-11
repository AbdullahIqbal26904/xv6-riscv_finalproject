#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{

char data[4] = "1234";
int final = k_sha256(data, 4);
printf("Value is encrpted %d\n", final);
exit(0);
}


