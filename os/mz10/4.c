#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

enum
{
    START_SIZE = 128
};

void redrum(int *pids, int size)
{
    for(int i = 0; i < size; i++) {
        kill(pids[i], SIGKILL);
    }
    while(wait(NULL) != -1){}
}

int main(int argc, char *argv[])
{
    int size = 0;
    int capacity = START_SIZE;
    pid_t *sons = calloc(capacity, sizeof(sons));
    if(sons == NULL) {
        return 1;
    }
    
    int fd[2];
    int read_fd = -1;
    
    for(int i = 1; i < argc; i++) {
        if(pipe(fd) == -1) {
            redrum(sons, size);
            free(sons);
            return 1;
        }
        
        pid_t pid = fork();
        if(pid == -1){
            redrum(sons, size);
            free(sons);
            return 1;
        }
        
        if(pid > 0) {
            sons[size++] = pid;
            if(capacity - size <= 1){
                capacity *= 2;
                pid_t *new_arr = realloc(sons, capacity * sizeof(*new_arr));
                if (new_arr == NULL) {
                    redrum(sons, size);
                    free(sons);
                    return 1;
                }
                sons = new_arr;
            }
            
            if(i > 1){
                close(read_fd);
            }
            if(i != argc - 1) {
                read_fd = fd[0];
            }else{
                close(fd[0]);
            }
            close(fd[1]);
        }else{
            if(i > 1) {
                if(dup2(read_fd, 0) == -1){
                    return 1;
                }
            }
            if(i != argc - 1) {
                if(dup2(fd[1], 1) == -1) {
                    return 1;
                }
            }
            
            if(i > 1){
                close(read_fd);
            }
            
            close(fd[0]);
            close(fd[1]);
            
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        
    }
    
    free(sons);
    while(wait(NULL) != -1){}
    return 0;
}
