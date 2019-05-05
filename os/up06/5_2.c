#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

enum
{
    START_CAPACITY = 128
};

struct strvec
{
    char **v;
    size_t a, u;
};

int traverse(const char *dir)
{
    DIR *d = opendir(dir);
    if (!d) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        return -1;
    }
    struct dirent *dd;
    char path[PATH_MAX];
    char *
    struct strvec vv = { malloc(16 * sizeof(vv.v[0])), 16, 0 };
    int size = 0;
    int capacity = START_SIZE;
    
    while ((dd = readdir(d))) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, ".."))
            continue;
        int slen = snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name);
        if (slen + 1 > sizeof(path)) {
            continue;
        }
        
        struct stat stb;
        if (lstat(path, &stb) < 0) continue;
        if (S_ISDIR(stb.st_mode)) {
            if (vv.u == vv.a) {
                vv.v = realloc(vv.v, (vv.a *= 2) * sizeof(vv.v[0]));
            }
            printf("cd %s\n", dd -> d_name);
            vv.v[vv.u++] = strdup(dd->d_name);
        }
    }
    closedir(d);
    for (size_t i = 0; i < vv.u; ++i) {
        snprintf(path, sizeof(path), "%s/%s", dir, vv.v[i]);
        traverse(path);
        free(vv.v[i]);
    }
    if(!first_enter) {
        printf("cd ..");
    }
    free(vv.v);
    return 0;
}

int main(int argc, char *argv[])
{
    traverse(argv[1]);
    return 0;
}
