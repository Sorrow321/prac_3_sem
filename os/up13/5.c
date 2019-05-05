#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

unsigned char getbit(unsigned char *mem, unsigned long long num) {
    return (mem[num >> 3] >> (7u & num)) & 1;
}

void zerobit(unsigned char *mem, unsigned long long num) {
    mem[num >> 3] &= ~(1u << (7u & num));
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        return 0;
    }
    // находим наименьший по размеру файл
    struct stat stb;
    stat(argv[1], &stb);
    off_t min_size = stb.st_size;
    int min_i = 1;
    for(int i = 2; i < argc; i++) {
        stat(argv[i], &stb);
        if(stb.st_size < min_size) {
            min_size = stb.st_size;
            min_i = i;
        }
    }
    
    unsigned char *mem = mmap(NULL, min_size * 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(unsigned long long i = 0; i < min_size; i++){
        for(unsigned j = 0; j < 8; j++) {
            mem[i] |= 1u << j;
        }
    }
    
    for(int i = 1; i < argc; i++) {
        if(i == min_i) { // игнорируем файл наименьшего размера
            continue;
        }
        pid_t pid = fork();
        if(pid == -1){
            return 1;
        }
        if(!pid) {
            // с каким файлом работает этот процесс?
            int file_i = i;
            FILE *lfl = fopen(argv[file_i], "r");
            FILE *rfl = fopen(argv[min_i], "r");
            
            unsigned long long num = 0; // счетчик чисел в файле наименьшего размера
            
            int lhs, rhs;
            while(fscanf(rfl, "%d", &rhs) == 1) {
                int k;
                while((k = fscanf(lfl, "%d", &lhs)) == 1 && lhs < rhs) {
                }
                if(k != 1) {
                    while(fscanf(rfl, "%d", &rhs) == 1) {
                        zerobit(mem, num);
                        num++;
                    }
                    zerobit(mem, num);
                    return 0;
                }
                if(lhs == rhs) {
                    num++;
                    continue;
                }else{
                    zerobit(mem, num);
                    num++;
                    int l;
                    while((l = fscanf(rfl, "%d", &rhs) == 1) && lhs > rhs) {
                        zerobit(mem, num);
                        num++;
                    }
                    if(l != 1){
                        zerobit(mem, num);
                    }
                }
            }
            fclose(lfl);
            fclose(rfl);
            return 0;
        }
    }
    
    for(int i = 0; i < argc - 2; i++) {
        wait(NULL);
    }
    FILE *f = fopen(argv[min_i], "r");
    int a;
    unsigned long long i = 0, j = 0;
    while(fscanf(f, "%d", &a) == 1){
        if(getbit(mem, i)) {
            printf("%d ", a);
            j++;
        }
        i++;
    }
    if(j > 0) {
        printf("\n");
    }
    fclose(f);
    return 0;
}
