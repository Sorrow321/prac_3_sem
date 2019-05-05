#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <limits.h>


int main(int argc, char *argv[])
{
    int fd1 = open(argv[2], O_RDONLY);
    int fd2 = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0660);
    int fd3 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0660);
    
    pid_t pid = fork();
    if(pid == -1) {
        return 0;
    }else if(pid > 0){
        int status;
        wait(&status);
        printf("%d\n", status);
    }else{
        if(dup2(fd1, STDIN_FILENO) == -1 || dup2(fd2, STDOUT_FILENO) == -1 || dup2(fd3, STDERR_FILENO) == -1) {
            _exit(42);
        }
        execlp(argv[1], argv[1], NULL);
        _exit(42);
    }
    
    close(fd1);
    close(fd2);
    close(fd3);
    
    return 0;
}
