#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    A = 1103515245,
    C = 12345,
    MOD = 1u << 31,
    MAX_LEN = 33
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

char encode(int a){
    if(a <= 9){
        return '0' + a;
    }else if(a <= 35){
        return 'A' + a - 10;
    }
    return 'a' + a - 36;
}

unsigned decode(char a){
    if('0' <= a && a <= '9') {
        return a - '0';
    } else if('a' <= a && a <= 'z') {
        return 36 + a - 'a';
    }
    return 10 + a - 'A';
}

int main(void)
{
    char pw[MAX_LEN];
    scanf("%32s", pw);
    int len = strlen(pw);
    char first_sym = pw[0];
    int b = decode(first_sym);
    unsigned low_edge = (int)((double)MOD * ((double)b)/62.0 + 0.9999);
    unsigned upper_edge = (int)((double)MOD * ((double)(b+1))/62.0);
    
    char ans[MAX_LEN];
    ans[0] = first_sym;
    int goods = 0;
    for(unsigned i = low_edge; i < upper_edge; i++) {
        RandomGenerator *rr = random_create(i);
        
        int flag = 0;
        for(int j = 1; j < len; j++) {
            int b = rr->ops->next(rr);
            double c = (double)b/(double)MOD;
            c *= 62.0;
            int d = (int)c;
            char ch = encode(d);
            //printf("%c", ch);
            
            
            if(pw[j] != '.' && ch != pw[j]) {
                flag = 1;
                break;
            }else{
                ans[j] = ch;
            }
        }
        //printf("\n");
        if(!flag) {
            goods++;
        }
        
        rr->ops->destroy(rr);
    }
    ans[len] = 0;
    if(goods == 1) {
        printf("%s\n", ans);
    }else{
        printf("#\n");
    }
    
    /*
    RandomGenerator *rr = random_create(128123812);
    
    for (int j = 0; j < 1; ++j) {
        int b = rr->ops->next(rr);
        printf("b = %d\n", b);
        double c = (double)b/(double)MOD;
        c *= 62.0;
        b = (int)c;
        printf("c = %c\n", uncode(b));
        printf("%.3lf <= b < %.3lf\n", (double)MOD * ((double)b)/62.0, (double)MOD * ((double)(b+1))/62.0);
    }
    rr->ops->destroy(rr);
    */
    return 0;
}
