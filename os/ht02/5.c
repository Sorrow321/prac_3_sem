#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

enum
{
    DEC_BASE = 10,
    BUF_SIZE = 16
};

int main(void)
{
    char buffer[BUF_SIZE], sign = '+';
    unsigned long long sum = 0, bufnum = 0;
    ssize_t r;
    while((r = read(STDIN_FILENO, &buffer, sizeof(buffer)))) {
        if(r == -1) {
            return 1;
        }

        for(int i = 0; i < r; i++) {
            if(buffer[i] == '+') {
                sign = '+';
            }else if(buffer[i] == '-') {
                sign = '-';
            }else if(isspace((unsigned char)buffer[i])) {
                if(sign == '+') {
                    sum += bufnum;
                }else{
                    sum -= bufnum;
                }
                sign = '+';
                bufnum = 0;
            }else{
                bufnum = bufnum * DEC_BASE + (buffer[i] - '0');
            }
        }
    }

    printf("%lld\n", (long long)sum);
    return 0;
}
