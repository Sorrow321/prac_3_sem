#include <stdio.h>
#include <signal.h>

int c;

void handler(int s)
{
    printf("sigint c: %d\n", c);
}

int main()
{
    signal(SIGINT, handler);
    int a,b,c;
    while((c = scanf("%d%d", &a, &b)) == 2) {
        printf("%d\n", a / b);
    }
    return 0;
}
