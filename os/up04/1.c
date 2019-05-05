#include <stdio.h>
#include <inttypes.h>

int_least32_t get_reverse(int_least32_t a, int_least32_t n)
{
    int_least32_t sign = 1;
    int_least32_t mask = 1;
    mask <<= (n - 1);
    if(a & mask) {
        sign = -1;
    }
    mask = ~mask;
    return sign * (a & mask);
}

int main()
{
    int_least32_t n;
    int w;
    scanf("%"PRIo32"%o", &n, &w);
    int_least32_t pow = 1 << n;

    for(int_least32_t i = 0; i < pow; i++) {
        int_least32_t p = get_reverse(i, n);
        printf("|%*"PRIo32"|%*"PRId32"|%*"PRId32"|\n", w, i, w, i, w, p);
    }

    return 0;
}
