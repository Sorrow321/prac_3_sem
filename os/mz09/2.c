#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int mysys(const char *str)
{
    pid_t pid = fork();
    if(pid == -1) {
        return -1;
    }else if(pid > 0){
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if(WIFSIGNALED(status)) {
            return 128 + WTERMSIG(status);
        }
    }else{
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    }
    return 0;
}
