#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

enum { MSGKEY = 655363 };
enum { COUNT = 2 };

struct Message
{
    long mtype;
    int value;
};

void work(int msg_id, int prc_id)
{
    while(1) {
        struct Message mm;
        int r = msgrcv(msg_id, &mm, sizeof(mm) - sizeof(mm.mtype), prc_id + 1, 0);
        if(r < 0){
            fprintf(stderr, "msgrcv: %s\n", strerror(errno));
            break;
        }
        printf("%d %d\n", prc_id + 1, mm.value++);
        
        mm.mtype = (prc_id + 1 ) % COUNT + 1;
        msgsnd(msg_id, &mm, sizeof(mm) - sizeof(mm.mtype), 0);
    }
}

void handl(int sigid){}

int main(void)
{  
    int msg_id = msgget(MSGKEY, IPC_CREAT | IPC_EXCL | 0600);
    
    if(msg_id < 0){
        fprintf(stderr, "msgget: %s\n", strerror(errno));
        exit(1);
    }
    
    for(int i = 0; i < COUNT; i++) {
        if(!fork()) {
            work(msg_id, i);
            _exit(0);
        }
    }
    struct sigaction sa = { .sa_handler = handl, .sa_flags = SA_RESTART };
    sigaction(SIGINT, &sa, NULL);
    {
        struct Message mm = { .mtype = 1, .value = 42 };
        msgsnd(msg_id, &mm, sizeof(mm) - sizeof(mm.mtype), 0);
    }
    
    while(wait(NULL) > 0) {}
    
    msgctl(msg_id, IPC_RMID, NULL);
    printf("Father killed queue.\n");
    return 0;
}
