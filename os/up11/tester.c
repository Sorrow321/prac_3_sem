#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int fd[2];
    pipe(fd);
    
    FILE *f1 = fdopen(fd[0], "r");
    FILE *f2 = fdopen(fd[1], "w");
    setbuf(f1, NULL);
    setbuf(f2, NULL);
    
    fprintf(f2, "10 20 ");
    
    int c, d;
    fscanf(f1, "%d %d", &c, &d);
    printf("c: %d, d: %d", c, d);
    
    fclose(f1);
    fclose(f2);
    return 0;
}
