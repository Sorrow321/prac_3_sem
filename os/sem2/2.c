#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
    unsigned long long sum_1 = 0;
    unsigned long long sum_2 = 0;
    off_t offset;
    sscanf(argv[2], "%ld", &offset);
    int fd = open(argv[1], O_RDONLY, 0600);
    if(fd == -1) {
        return 1;
    }
    unsigned short size;
    while (offset) {
        lseek(fd, offset, SEEK_CUR);
        ssize_t r = read(fd, &size, sizeof(size));
        if(r == -1) {
            return 1;
        }
        lseek(fd, size * sizeof(char), SEEK_CUR);
        sum_1++;
        sum_2 += size;
        r = read(fd, &offset, sizeof(offset));
        if(r == -1) {
            return 1;
        }
    }
    printf("%llu %llu\n", sum_1, sum_2);
    close(fd);
    return 0;
}
