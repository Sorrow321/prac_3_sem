#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

enum
{
    ARGNUM = 2,
    BITS_IN_BYTE = 8,
    UNSIGINTSIZE = sizeof(unsigned)
};


int main(int argc, char *argv[])
{
    if(argc < ARGNUM){
        return 1;
    }

    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(file == -1){
        return 1;
    }
    unsigned int a;

    while(scanf("%u", &a) == 1){
        unsigned char buffer[UNSIGINTSIZE];
        for(int i = 0; i < sizeof(a); i++) {
            buffer[sizeof(a) - i - 1] = a;
            a >>= BITS_IN_BYTE;
        }
        ssize_t w = write(file, &buffer, sizeof(buffer));
        if(w == -1){
            return 1;
        }
    }
    close(file);
    return 0;
}
