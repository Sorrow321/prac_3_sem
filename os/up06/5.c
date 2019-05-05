#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    START_SIZE = 128,
    READ_FLAG = 4
};

int comparator(const void *lhs, const void *rhs) 
{
    if(strcasecmp(*((char **)lhs), *((char **)rhs)) > 0) {
        return 1;
    }
    return -1;
}

void view_dir(char *path)
{
    DIR * d = opendir(path);    
    if(!d) {
        return;
    }
    
    char **darr = (char**)calloc(START_SIZE, sizeof(*darr));
    unsigned capacity = START_SIZE;
    unsigned size = 0;
    static char new_path[PATH_MAX + 2] = {};
    strcpy(new_path, path);
    struct dirent *entry;
    unsigned path_len = strlen(new_path);
    while((entry = readdir(d))) {
        if( strncmp (entry -> d_name, ".", 2) != 0 && strncmp (entry -> d_name, "..", 3) != 0) {
            struct stat stb = {};
            snprintf (new_path + path_len, PATH_MAX + 2, "/%s", entry -> d_name);
            if (!lstat(new_path, &stb) && S_ISDIR(stb.st_mode) && !access(new_path, READ_FLAG)) {
                darr[size++] = strdup(entry -> d_name);
                if(capacity - size < 2) {
                    capacity *= 2;
                    char **new_arr = realloc(darr, capacity * sizeof(*new_arr));
                    if (new_arr == NULL) {
                        free(darr);
                        return;
                    }
                    darr = new_arr;
                }
            }
        }
    }
    closedir(d);
    qsort(darr, size, sizeof(*darr), comparator);
    
    for(unsigned i = 0; i < size; i++) {
        int len = snprintf(new_path + path_len, PATH_MAX + 2, "/%s", darr[i]);
        if(len + 1 > PATH_MAX) {
            continue;
        }
        printf("cd %s\n", darr[i]);
        free(darr[i]);
        view_dir (new_path);
        printf("cd ..\n");
    }
    free(darr);
}

int main(int argc, char *argv[])
{
    view_dir(argv[1]);
    return 0;
}
