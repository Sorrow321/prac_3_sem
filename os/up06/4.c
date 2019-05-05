#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    DIR * d = opendir(argv[1]);
    if(d == NULL) {
        return 1;
    }
    unsigned long long sum = 0;
    struct dirent * entry;
    while((entry = readdir(d))) {
        char name[PATH_MAX + 2];
        strcpy(name, argv[1]);
        strcat(name, "/");
        strcat(name, entry -> d_name);
        struct stat stb = {};
        stat(name, &stb);
        if(S_ISREG(stb.st_mode) && stb.st_uid == getuid() && 
            'A' <= entry -> d_name[0] && entry -> d_name[0] <= 'Z') {
            sum += stb.st_size;
        }
    }
    printf("%llu\n", sum);
    closedir(d);
    return 0;
}
