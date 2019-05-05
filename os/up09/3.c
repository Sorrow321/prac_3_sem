#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum
{
    ARGNUM = 4
};

int get_proc_exit(char *name)
{
    pid_t pid = fork();
    
    if(pid > 0) {
        int status;
        wait(&status);
        if(WIFEXITED(status) && !WEXITSTATUS(status)) {
            return 1;
        }
    }else if(!pid){
        execlp(name, name, NULL);
        _exit(0);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc >= ARGNUM && (get_proc_exit(argv[1]) || get_proc_exit(argv[2])) && get_proc_exit(argv[3])) { 
        return 0;
    }
    return 1;
}
