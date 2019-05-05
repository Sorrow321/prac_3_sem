#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>

int main(void){
    printf("parent\n");
    for(int i = 0; i < 5; i++){
        pid_t pid = fork();
        if(!pid){
            printf("child %d\n", i);
            if(i == 50) {
                *((int*) i) = 10;
            }else {
                exit(0 + i);
            }
        }
    }
    int pid, status;
    while((pid = wait(&status)) > 0) {
        if( WIFEXITED(status) && !WEXITSTATUS(status)) {
            printf("success: ");
        }
        if(WIFEXITED(status)) {
            printf("pid = %d, exit code = %d\n", pid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)) {
            printf("pid = %d, signal = %d\n", pid, WTERMSIG(status));
        }
    }
    
    return 0;
}
