#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

unsigned long long hash_function(unsigned long long);

int main(int argc, char *argv[])
{
    if(argc < 4) {
        return 1;
    }
    char name[PATH_MAX];
    unsigned long long h_0, count;
    sscanf(argv[1], "%s", name);
    sscanf(argv[2], "%llx", &h_0);
    sscanf(argv[3], "%llu", &count);
    
    int file = open(name, O_RDWR | O_CREAT, 0660);
    if(file == -1) {
        return 1;
    }
    off_t p1 = lseek(file, 0, SEEK_END);
    unsigned long long h = h_0;
    for(unsigned long long i = 0; i < count; i++) {
        lseek(file, p1 + (count - i - 1) * sizeof(h), SEEK_SET);
        ssize_t w = write(file, &h, sizeof(h));
        if(w == -1) {
            return 1;
        }
        h = hash_function(h);
    }
    
    close(file);
    return 0;
}
