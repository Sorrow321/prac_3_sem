#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 2281488 };

int main(void)
{
    int sem = semget(KEY, 2, O_CREAT | O_EXCL | 0600);
    if(sem < 0){
        fprintf(stderr, "semget: %s\n", strerror(errno));
        return 1;
    }
    
