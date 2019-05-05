#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    int pid = fork();
    if(!pid) {
        execlp("ls", "ls", "-l", "/", NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
