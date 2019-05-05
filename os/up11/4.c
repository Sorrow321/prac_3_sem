#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

volatile char flag;
volatile char father_flag;

void sighandler(int sigid)
{
    flag = 1;
}

void father_handler(int sigid)
{
    father_flag++;
}

int main(int argc, char *argv[])
{
    int N;
    sscanf(argv[1], "%d", &N);
    if(N == 1) {
        return 0;
    }
    
    sigset_t ss1, ss2;
    sigemptyset(&ss1);
    sigaddset(&ss1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss1, &ss2);
    
    struct sigaction saa = { .sa_handler = father_handler, .sa_flags = SA_RESTART };
    sigaction(SIGUSR1, &saa, NULL);
    
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        return 1;
    }
    
    if(!pid1){
        FILE *pwr = fdopen(fd[1], "w");
        FILE *prd = fdopen(fd[0], "r");
        setbuf(pwr, NULL);
        setbuf(prd, NULL);
        
        sigset_t s1, s2;
        sigemptyset(&s1);
        sigaddset(&s1, SIGUSR1);
        sigprocmask(SIG_BLOCK, &s1, &s2);
        
        struct sigaction sa = { .sa_handler = sighandler, .sa_flags = SA_RESTART };
        sigaction(SIGUSR1, &sa, NULL);
        
        kill(getppid(), SIGUSR1);
        
        int a = 1;
        pid_t mypid = getpid(); 
        while(1){
            while(!flag){
                sigsuspend(&ss2);
            }
            flag = 0;
            pid_t brother;
            if(fscanf(prd, "%d %d", &brother, &a) != 2) {
                fclose(prd);
                fclose(pwr);
                break;
            }
            
            printf("1 %d\n", a);
            fflush(stdout);
            
            a++;
            if(a >= N) {
                fclose(prd);
                fclose(pwr);
                kill(brother, SIGUSR1);
                break;
            }
            
            fprintf(pwr, "%d %d ", mypid, a);
            if(a == N - 1){
                fclose(pwr);
            }
            kill(brother, SIGUSR1);
        }
        return 0;
    }
    
    
    pid_t pid2 = fork();
    if(pid2 == -1){
        return 1;
    }
    if(!pid2){
        FILE *pwr = fdopen(fd[1], "w");
        FILE *prd = fdopen(fd[0], "r");
        setbuf(pwr, NULL);
        setbuf(prd, NULL);
        
        sigset_t s1, s2;
        sigemptyset(&s1);
        sigaddset(&s1, SIGUSR1);
        sigprocmask(SIG_BLOCK, &s1, &s2);
        
        struct sigaction sa = { .sa_handler = sighandler, .sa_flags = SA_RESTART };
        sigaction(SIGUSR1, &sa, NULL);
        
        kill(getppid(), SIGUSR1);
        
        int a = 1;
        pid_t mypid = getpid();
        while(1){
            while(!flag){
                sigsuspend(&ss2);
            }
            flag = 0;
            pid_t brother;
            if(N == 2 || fscanf(prd, "%d %d", &brother, &a) != 2) {
                fclose(prd);
                fclose(pwr);
                break;
            }
            printf("2 %d\n", a);
            fflush(stdout);
            
            a++;
            if(a >= N) {
                fclose(prd);
                fclose(pwr);
                kill(brother, SIGUSR1);
                break;
            }
            
            fprintf(pwr, "%d %d ", mypid, a);
            if(a == N - 1){
                fclose(pwr);
            }
            kill(brother, SIGUSR1);
        }
        return 0;
    }
    
    while(father_flag != 2) { // оба сына готовы принимать сигналы
        sigsuspend(&ss2);
    }
    
    FILE *pwr = fdopen(fd[1], "w");
    setbuf(pwr, NULL);
    
    fprintf(pwr, "%d 1 ", pid2);
    fclose(pwr);
    close(fd[0]);
    
    kill(pid1, SIGUSR1);
    wait(NULL);
    wait(NULL);
    
    printf("Done\n");
    fflush(stdout);
    return 0;
}
