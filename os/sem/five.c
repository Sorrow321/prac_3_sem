#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

enum { VALCOUNT = 1024 * 1024 };

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    if(fd < 0)return 1;
    
    unsigned long long val = 0;
    size_t size = VALCOUNT * sizeof(val);
    ftruncate(fd, VALCOUNT * sizeof(val));
    
    size_t pagesize = getpagesize();
    void *vptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(vptr == MAP_FAILED) return 1;
    
    
    unsigned long long *lptr = vptr;
    for(int i = VALCOUNT - 1; i >= 0; --i) {
        lptr[i] = val++;
    }
    munmap(vptr, size);
    
    close(fd);
    return 0;
}
