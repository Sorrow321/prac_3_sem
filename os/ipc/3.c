#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

enum { KEY = 2281488 };

struct kekus{
    long type;
    int num;
};

int main(void)
{
    int msgid = msgget(KEY, IPC_CREAT | IPC_EXCL | 0600);
    int a = 10;
    int b = 33;
    
    struct kekus mes;
    
    mes.type = 1;
    mes.num = a;
    
    msgsnd(msgid, &mes, sizeof(mes) - sizeof(mes.type), 0);
    mes.num = b;
    msgsnd(msgid, &mes, sizeof(mes) - sizeof(mes.type), 0);
    
    if(!fork()){
        int c, d;
        msgrcv(msgid, &mes, sizeof(mes) - sizeof(mes.type), 1, 0);
        c = mes.num;
        printf("First: %d\n", c);
        msgrcv(msgid, &mes, sizeof(mes) - sizeof(mes.type), 1, 0);
        d = mes.num;
        printf("Second: %d\n", d);
        c += d;
        
        mes.type = 2;
        mes.num = c;
        msgsnd(msgid, &mes, sizeof(mes) - sizeof(mes.type), 0);
        return 0;
    }
    msgrcv(msgid, &mes, sizeof(mes) - sizeof(mes.type), 2, 0);
    msgctl(msgid, IPC_RMID, NULL);
    printf("Sum: %d\n", mes.num);
    return 0;
}
