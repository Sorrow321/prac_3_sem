#include <stdio.h>

enum
{
    BYTES_NUM = 8,
    BITS_IN_BYTE = 8
};

void transform(unsigned bytes[])
{
    for(int i = 0; i < BYTES_NUM; i++) {
        for(int j = i; j < BITS_IN_BYTE; j++) {
            unsigned bit1 = bytes[i] >> j & 1;
            unsigned bit2 = bytes[j] >> i & 1;
            if(bit1) {
                bytes[j] |= 1 << i;
            }else{
                bytes[j] &= ~(1 << i);
            }
            if(bit2) {
                bytes[i] |= 1 << j;
            }else{
                bytes[i] &= ~(1 << j);
            }
        }
    }
}

int main(void)
{
    unsigned bytes[BYTES_NUM];
    int i = 0;
    while(scanf("%x", &bytes[i]) == 1) {
        i++;
        
        if(i == BYTES_NUM) {
            transform(bytes);
            i = 0;
            for(int j = 0; j < BYTES_NUM; j++) {
                printf("%x ", bytes[j]);
            }
        }
    }
    printf("\n");
    
    return 0;
}
