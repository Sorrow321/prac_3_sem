#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

int main(void){
    pid_t pid = fork();
    if(pid > 0) {
        // parent
        wait(NULL);
        printf("parent\n");
    } else if(!pid) {
        // child
        printf("child\n");
    }
    
    return 0;
}
