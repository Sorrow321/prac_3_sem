#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    char *cmd1 = argv[1];
    char *cmd2 = argv[2];
    char *cmd3 = argv[3];
    char *cmd4 = argv[4];
    char *file1 = argv[5];
    
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    
    if(!pid1) {
        dup2(fd[1], 1);
        pid_t pid2 = fork();
        if(pid2 == -1){
            return 1;
        }
        close(fd[0]);
        close(fd[1]);
        if(pid2 > 0){
            int status;
            wait(&status);
            if(WIFEXITED(status) && !WEXITSTATUS(status)) {
                execlp(cmd2, cmd2, NULL);
                _exit(1);
            }
            _exit(1);
        }else{
            execlp(cmd1, cmd1, NULL);
            _exit(1);
        }
    }
    
    pid_t pid3 = fork();
    if(pid3 == -1){
        return 1;
    }
    if(!pid3) {
        dup2(fd[0], 0);
        int fdd = open(file1, O_CREAT | O_TRUNC | O_WRONLY, 0600);
        dup2(fdd, 1);
        close(fd[0]);
        close(fd[1]);
        
        pid_t pid4 = fork();
        if(pid4 == -1){
            return 1;
        }
        if(pid4 > 0){
            wait(NULL);
            execlp(cmd4, cmd4, NULL);
            _exit(1);
        }else{
            execlp(cmd3, cmd3, NULL);
            _exit(1);
        }
        _exit(1);
    }
    
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}
