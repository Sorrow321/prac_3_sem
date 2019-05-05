#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    FATHER = 0,
    SON = 1,
    GRANDSON = 2
};

int main(void)
{
    int fd[2];
    if(pipe(fd)){
        return 1;
    }
    
    for(int i = 0; i < 3; i++){
        pid_t pid = fork();
        if(pid == -1) {
            return 1;
        }
        if(pid > 0) {
            close(fd[1]);
            wait(NULL);
            time_t tim;
            ssize_t r = read(fd[0], &tim, sizeof(tim));
            if(r == -1) {
                return 1;
            }
            struct tm *times = localtime(&tim);
            switch(i) {
            case FATHER:
                printf("Y:%04d\n", times -> tm_year + 1900);
                break;
            case SON:
                printf("M:%02d\n", times -> tm_mon + 1);
                break;
            case GRANDSON:
                printf("D:%02d\n", times -> tm_mday);
                break;
            }
            
            fflush(stdout);
            close(fd[0]);
            break;
        }else{
            if(i == GRANDSON) {
                close(fd[0]);
                time_t tim = time(NULL);
                for(int j = 0; j < 3; j++) {
                    ssize_t w = write(fd[1], &tim, sizeof(tim));
                    if(w == -1) {
                        return 1;
                    }
                }
                close(fd[1]);
            }
        }
    }
    return 0;
}
