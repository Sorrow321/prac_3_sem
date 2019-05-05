#include <stdlib.h>
#include <stdio.h>
#include <math.h>

enum
{
    ARGNUM = 5
};

int main(int argc, char *argv[])
{
    if(argc < ARGNUM){
        return 1;
    }
    int count, low, high;
    unsigned seed;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &low);
    sscanf(argv[3], "%d", &high);
    sscanf(argv[4], "%u", &seed);
    srand(seed);
    
    for(int i = 0; i < count; i++) {
        unsigned a = rand();
        double b = a / ((double)RAND_MAX + 1);
        int c = (int)((high - low)*b) + low;
        
        printf("%d\n", c);
    }
    return 0;
}
