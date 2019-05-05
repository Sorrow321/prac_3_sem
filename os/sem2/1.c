#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    DIR * d = opendir(argv[1]);
    if(d == NULL) {
        return 1;
    }
    
    struct dirent * entry;
    while(entry = readdir(d)) {
        
        char name[PATH_MAX];
        strcpy(name, argv[1]);
        strcat(name, entry -> d_name);
        struct stat stb;
        lstat(name, &stb);
        
        if((stb.st_mode & S_IFMT) == S_IFREG) {
            int len = strlen(entry -> d_name);
            if((entry -> d_name)[len - 1] == '~') {
                unlink(name);
            }
            if(len > 3 && !strcmp((entry -> d_name) + len - 4, ".bak")) {
                unlink(name);
            }
        }else if((stb.st_mode & S_IFMT) == S_IFDIR){
            int i = 0;
            char flag = 1;
            DIR * dir2 = opendir(name);
            while(1) {
                struct dirent * entr2;
                entr2 = readdir(dir2);
                i++;
                if(i == 2) {
                    flag = 2;
                    break;
                }
            }
            closedir(dir2);
            if(flag == 2) {
                rmdir(name);
            }
        }
    }
    closedir(d);
    return 0;
}
