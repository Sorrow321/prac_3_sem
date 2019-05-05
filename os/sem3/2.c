#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    pid_t pid = fork();
    if(pid > 0) {
        // parent
        usleep(1000);
        printf("parent\n");
        wait(NULL);
    } else if(!pid) {
        // child
        printf("child\n");
    }
    return 0;
}
