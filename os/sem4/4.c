#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int pid = fork();
    if(!pid) {
        /*
        int fd = open("input.txt", O_RDONLY, 0);
        dup2(fd, 0);'
        close(fd);
        */
        
        
        //close(0);
        //int fd = open("input.txt", O_RDONLY, 0);
        
        execl("add.py", "add.py", NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
