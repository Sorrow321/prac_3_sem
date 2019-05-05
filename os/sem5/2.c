#include <stdio.h>
#include <signal.h>

void handler(int s)
{
    int saved = errno;
    write(1, "sigint\n", 7);
    errno = saved;
}

int main()
{
    struct sigaction sa = { .sa_flags = SA_RESTART, .sa_handler = handler };
    sigaction(SIGINT, &sa, NULL);
    int a,b;
    while(scanf("%d%d", &a, &b) == 2) {d
        printf("%d\n", a / b);
    }
    return 0;
}
