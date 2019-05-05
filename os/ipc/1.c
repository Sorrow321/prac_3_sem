#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

enum { KEY = 2281488 };

struct Msg{
    long type;
    int number;
};


void work(int fd, int num)
{
    struct Msg msg;
    while(1) {
        if(msgrcv(fd, &msg, sizeof(msg) - sizeof(msg.type), num, 0) < 0){
            exit(0);
        }
        printf("%d %d\n", num, msg.number);
        fflush(stdout);
        msg.type = num % 2 + 1;
        msg.number++;
        if(msg.number > 100){
            exit(0);
        }
        msgsnd(fd, &msg, sizeof(msg) - sizeof(msg.type), 0);
    }
}

int main(void)
{
    int fd = msgget(KEY, IPC_CREAT | IPC_EXCL | 0600);
    if(fd == -1){
        fprintf(stderr, "msgget: %s\n", strerror(errno));
        return 1;
    }
    
    struct Msg message = {.type = 1, .number = 1 };
    
    for(int i = 1; i < 3; i++) {
        if(!fork()){
            work(fd, i);
        }
    }
    msgsnd(fd, &message, sizeof(message) - sizeof(message.type), 0);
    wait(NULL);
    msgctl(fd, IPC_RMID, NULL);
    wait(NULL);
    
    return 0;
}
