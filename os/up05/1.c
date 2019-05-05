#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int f = open(argv[1], O_RDWR);
    if(f == -1) {
        return 1;
    }

    ssize_t r;
    long long bufnum;
    long long min = LLONG_MAX;
    off_t pos = 0;
    off_t min_pos = 0;
    int empty_flag = -1;
    while((r = read(f, &bufnum, sizeof(bufnum)))) {
        if(r == -1) {
            return 1;
        }
        empty_flag = 1;
        if(bufnum < min) {
            min = bufnum;
            min_pos = pos;
        }
        pos += r;
    }
    if(empty_flag == -1) {
        close(f);
        return 0;
    }
    if(min == LLONG_MIN) {
        close(f);
        return 0;
    }
    min = -min;
    off_t ls = lseek(f, min_pos, SEEK_SET);
    if(ls == (off_t) -1){
        return 1;
    }
    
    ssize_t w = write(f, &min, sizeof(min));
    if(w == -1) {
        return 1;
    }
    close(f);
    return 0;
}
