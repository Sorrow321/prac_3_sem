#include <stdio.h>
#include <stdlib.h>

struct pair
{
    int x;
    int y;
};

int main(void)
{
    int n;
    scanf("%d", &n);
    struct pair numbers[100];
    int space[100] = {};
    for(int i = 0; i < n; i++){
        scanf("%d%d", &numbers[i].x, &numbers[i].y);
        space[i] = numbers[i].y + (i? space[i - 1] : 0);
    }
    
    int m;
    unsigned seed;
    scanf("%d%u", &m, &seed);
    srand(seed);
    
    for(int i = 0; i < m; i++){
        unsigned a = rand();
        
        double b = a / (double)(RAND_MAX + 1.0);
        b *= 100.0;
        
        int sum = 0;
        for(int j = 0; j < n; j++){
            if(b < space[j]) {
                if(!j) {
                    printf("%d ", numbers[0].x);
                    break;
                }else if(space[j - 1] <= b){
                    printf("%d ", numbers[j].x);
                    break;
                }
            }
        }
    }
    printf("\n");
    return 0;
}
