#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd1 = open(argv[1], O_RDONLY, 0600);
    int fd2 = open(argv[2], O_RDONLY, 0600);
    int fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(fd1 == -1 || fd2 == -1 || fd3 == -1) {
        return 1;
    }
    
    double d1, d2;
    ssize_t r1 = read(fd1, &d1, sizeof(d1));
    ssize_t r2 = read(fd2, &d2, sizeof(d2));
    while(r1 || r2) {
        if(r1 == 0) {
            ssize_t w1 = write(fd3, &d2, sizeof(d2));
            if(w1 == -1) {
                return 1;
            }
            continue;
        }
        if(r2 == 0) {
            ssize_t w2 = write(fd3, &d1, sizeof(d1));
            if(w2 == -1) {
                return 1;
            }
            continue;
        }
        if(d1 < d2) {
            ssize_t w3 = write(fd3, &d1, sizeof(d1));
            if(w3 == -1) {
                return 1;
            }
            r1 = read(fd1, &d1, sizeof(d1));
            if(r1 == -1) {
                return 1;
            }
        }else{
            ssize_t w3 = write(fd3, &d2, sizeof(d2));
            if(w3 == -1) {
                return 1;
            }
            r2 = read(fd2, &d2, sizeof(d2));
            if(r2 == -1) {
                return 1;
            }
        }
    }
    
    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}
