#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

enum
{
    ARGNUM = 6
};

int main(int argc, char *argv[])
{
    if(argc < ARGNUM){
        return 1;
    }
    char *cmd1 = argv[1];
    char *cmd2 = argv[2];
    char *cmd3 = argv[3];
    char *file1 = argv[4];
    char *file2 = argv[5];
    
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    
    if(!pid1) {
        close(fd[0]);
        if(dup2(fd[1], 1) == -1){
            _exit(1);
        }
        close(fd[1]);
        pid_t pid2 = fork();
        if(pid2 == -1){
            return 1;
        }
        if(pid2 > 0){
            int status;
            wait(&status);
            if(WIFEXITED(status) && !WEXITSTATUS(status)) {
                execlp(cmd2, cmd2, NULL);
                _exit(1);
            }
            _exit(0);
        }else{
            int fdd = open(file1, O_CREAT | O_RDONLY, 0600);
            if(dup2(fdd, 0) == -1){
                _exit(1);
            }
            execlp(cmd1, cmd1, NULL);
            _exit(1);
        }
    }
    
    pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    if(!pid1){
        close(fd[1]);
        int fdd = open(file2, O_CREAT | O_APPEND | O_WRONLY, 0600);
        if(dup2(fd[0], 0) == -1 || dup2(fdd, 1) == -1) {
            _exit(1);
        }
        close(fd[0]);
        execlp(cmd3, cmd3, NULL);
        _exit(1);
    }
    
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    
    return 0;
}
