#include <sys/mman.h>
#include <stdio.h>

int main(void)
{
    char *mem = mmap(NULL, 100*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sprintf(mem, "Hey cruel world!");
    printf("%s\n", mem);
    return 0;
}
