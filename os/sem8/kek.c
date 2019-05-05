#include <unistd.h>
#include <stdio.h>

int main(void)
{
    volatile int x = 1;
    pid_t pid = fork();
    if(pid > 0)
        x = 2;
    else
        x = 3;
    printf("%p\n", &x);
    fflush(stdout);
    return 0;
}
