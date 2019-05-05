#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

enum
{
    ARGNUM = 3
};

int main(int argc, char *argv[])
{
    const char *temp_fdir = getenv("XDG_RUNTIME_DIR");
    if(!temp_fdir) {
        temp_fdir = getenv("TMPDIR");
        if(!temp_fdir) {
            temp_fdir = "/tmp";
        }
    }
    
    char file_name[PATH_MAX + 2] = {};
    snprintf(file_name, PATH_MAX + 2, "%s/%d.py", temp_fdir, getpid());
    int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0744);
    if(fd == -1){
        return 1;
    }
    FILE *file = fdopen(fd, "w");
    
    fprintf(file, "#!\npython3 -c \"print(");
    for(int i = 1; i < argc; i++) {
        fprintf(file, "%s*", argv[i]);
    }
    fprintf(file, "1)\"\nrm $0");
    fclose(file);
    close(fd);
    
    execlp("sh", "sh", "-c", file_name, NULL);
    
    return 1;
}
