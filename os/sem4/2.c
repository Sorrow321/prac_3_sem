#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int pid = fork();
    if(!pid) {
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
