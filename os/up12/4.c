#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char *out = argv[1];
    int mod;
    sscanf(argv[2], "%d", &mod);
    
    char temp_name[32];
    sprintf(temp_name, "%d.c", getpid());
    int fd = open(temp_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if(fd == -1){
        return 1;
    }
    FILE *f = fdopen(fd, "w");
    const char cntnt[] = "#include <stdio.h>\nint main(void)\n{\nint a;\nscanf(\"%d\", &a);\n\
    printf(\"%d\\n\", (a*((unsigned long long)a+1)/2)%";
    const char cntnt2[] = ");\nreturn 0;\n}";
    fprintf(f, "%s %d %s", cntnt, mod, cntnt2); 
    fclose(f);
    pid_t pid = fork();
    if(pid == -1){
        unlink(temp_name);
        return 1;
    }
    if(!pid) {
        execlp("gcc", "gcc", temp_name, "-o", out, NULL);
        _exit(1);
    }
    wait(NULL);
    unlink(temp_name);
    chmod(out, 0751);
    return 0;
}
