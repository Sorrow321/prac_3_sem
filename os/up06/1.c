#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    unsigned long long sum = 0;
    for(int i = 1; i < argc; i++) {
        struct stat info;
        if(lstat(argv[i], &info) == 0 && info.st_nlink == 1 && S_ISREG(info.st_mode)) {
            sum += info.st_size;
        }
    }
    printf("%llu\n", sum);
    return 0;
}
