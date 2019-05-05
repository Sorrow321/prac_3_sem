#include <stdio.h>
#include <unistd.h>

int main(void){
    printf("before fork\n");
    fflush(stdout);
    pid_t pid = fork();
    if(pid > 0) {
        // parent
        printf("parent\n");
    } else if(!pid) {
        // child
        // default pattern for fork()
        printf("child\n");
        execlp("lsaa", "ls", NULL);
        _exit(1); // <- обязательно _exit(1)
    }
    printf("after\n");
    return 0;
}
