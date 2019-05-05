#include <inttypes.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct Message
{
    long mtype;
    int64_t value[2];
};

int64_t absol(int64_t a){
    if(a >= 0){
        return a;
    }
    return -a;
}

int main(int argc, char *argv[])
{
    key_t key;
    int n;
    int64_t value1, value2, maxval;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%"SCNd64, &value1);
    sscanf(argv[4], "%"SCNd64, &value2);
    sscanf(argv[5], "%"SCNd64, &maxval);
    
    int msg_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);
    if(msg_id < 0){
        return 1;
    }
    
    for(int i = 0; i < n; i++) {
        pid_t pid = fork();
        if(pid == -1){
            signal(SIGTERM, SIG_IGN);
            msgctl(msg_id, IPC_RMID, NULL);
            kill(0, SIGTERM);
            return 1;
        }
        
        if(!pid) {
            while(1) {
                struct Message mes;
                int r = msgrcv(msg_id, &mes, sizeof(mes) - sizeof(mes.mtype), i + 1, 0);
                if(r < 0){
                    _exit(0);
                }
                printf("%d ""%"PRId64"\n", i, mes.value[0] + mes.value[1]);
                fflush(stdout);
                
                int64_t temp = mes.value[0] + mes.value[1];
                if(absol(temp) > maxval) {
                    _exit(0);
                }
                mes.value[0] = mes.value[1];
                mes.value[1] = temp;
                
                mes.mtype = (temp % n) + 1;
                if(msgsnd(msg_id, &mes, sizeof(mes) - sizeof(mes.mtype), 0) < 0){
                    _exit(0);
                }
            }
        }
    }
    struct Message mess;
    mess.mtype = 1;
    mess.value[0] = value1;
    mess.value[1] = value2;
    msgsnd(msg_id, &mess, sizeof(mess) - sizeof(mess.mtype), 0);
    wait(NULL);
    msgctl(msg_id, IPC_RMID, NULL);
    for(int i = 1; i < n; i++){
        wait(NULL);
    }
    return 0;
}
