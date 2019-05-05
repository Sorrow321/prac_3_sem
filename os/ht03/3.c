#include <stdio.h>
#include <stdlib.h>

enum
{
    A = 1103515245,
    C = 12345,
    MOD = 1 << 31
};


typedef struct RandomGenerator
{
    struct RandomOperations *ops;
    unsigned x;
} RandomGenerator;

typedef struct RandomOperations
{
    void (*destroy) (struct RandomGenerator* strs);
    int (*next) (struct RandomGenerator*);
} RandomOperations;

int next_f(struct RandomGenerator* str) {
    unsigned int r = A * str -> x + C;
    r %= MOD;
    str -> x  = r;
    return r;
}

void del_f(struct RandomGenerator* str) {
    free(str -> ops);
    free(str);
}

struct RandomGenerator *random_create(int seed)
{
    struct RandomGenerator *gen = (struct RandomGenerator*)calloc(1, sizeof(*gen));
    struct RandomOperations *op = (struct RandomOperations*)calloc(1, sizeof(*op));
    op -> next = next_f;
    op -> destroy = del_f;
    gen -> x = seed;
    gen -> ops = op;
    return gen;
}
