#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>

volatile int cur_num = 0;

void hand(int sid_id)
{
    cur_num--;
}

char *getline2(FILE *f, int max_sym)
{
    int sum  = 0;
    char *arr = (char*)calloc(128, sizeof(*arr));
    if (arr == NULL) {
        return NULL;
    }
    int capacity = 128;
    int size = 0;
    int a;

    while ((a = fgetc(f)) != EOF) {
        sum++;
        if(sum >= max_sym) {
            break;
        }
        if (a == '\n') {
            arr[size] = '\n';
            size++;
            break;
        }

        arr[size] = a;
        size++;
        if (capacity - size <= 2) {
            capacity *= 2;
            char *new_arr = realloc(arr,capacity * sizeof(*new_arr));
            if (new_arr == NULL) {
                free(arr);
                return NULL;
            }
            arr = new_arr;
        }
    }
    if (!size) {
        free(arr);
        return NULL;
    }
    char *new_arr =  realloc(arr, (size + 1) * sizeof(*new_arr));
    if (new_arr == NULL) {
        free(arr);
        return NULL;
    }
    arr = new_arr;
    arr[size] = 0;
    return arr;
}

int main(int argc, char *argv[])
{
    int N;
    sscanf(argv[1], "%d", &N);
    unsigned sum = 0;
    for(int i = 2; i < argc; i++){
        cur_num++;
        
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(pid > 0){
            
        }else{
            FILE *file = fopen(argv[i], "r");
            char path[PATH_MAX + 2];
            fscanf(file, "%s", path);
            execlp(path, path, NULL);
            _exit(1);
        }
        
        if(cur_num == N) {
            int status;
            while(wait(&status) != -1) {
                if(WIFEXITED(status) && !WEXITSTATUS(status)) {
                    sum++;
                }
            }
            cur_num = 0;
        }
    }
    int status;
    while(wait(&status) != -1) {
        if(WIFEXITED(status) && !WEXITSTATUS(status)) {
            sum++;
        }
    }
    
    printf("%u\n", sum);
    return 0;
}
