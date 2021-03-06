#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY, 0);
    if(fd < 0)return 1;
    
    
    struct stat stb;
    fstat(fd, &stb);
    if(!S_ISREG(stb.st_mode)){
        return 1;
    }
    off_t size = stb.st_size;
    if(!size)
        return 1;
    
    size_t pagesize = getpagesize();
    void *vptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(vptr == MAP_FAILED) return 1;
    
    enum { COUNT = '9' - '0' + 1 };
    
    long long counts[COUNT] = {};
    unsigned char *cptr = vptr;
    for(int i = 0; i < size; i++){
        unsigned v = cptr[i] - '0';
        if(v < COUNT){ ++counts[v];
            cptr[i] = '*';
        }   
    }
    
    for(int i = 0; i < COUNT; i++){
        printf("%d %lld\n", i, counts[i]);
    }
    close(file);
    return 0;
}
