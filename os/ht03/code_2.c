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


unsigned decode(char a){
    if('0' <= a && a <= '9') {
        return a - '0';
    } else if('a' <= a && a <= 'z') {
        return 36 + a - 'a';
    } else if('A' <= a && a <= 'Z') {
        return 10 + a - 'A';
    }
    return 1000;
}

int main(void)
{
    char pw[MAX_LEN];
    scanf("%32s", pw);
    int len = strlen(pw);
    for(int i = 0; i < len; i++) {
        if(pw[i] != '.' && decode(pw[i]) == 1000) {
            printf("#\n");
            return 0;
        }
    }
    char first_sym = pw[0];
    int b = decode(first_sym);
    unsigned low_edge = (int)((double)MOD * ((double)b)/62.0);
    unsigned upper_edge = (int)((double)MOD * ((double)(b+1))/62.0);
    
    unsigned nice_i;
    int goods = 0;
    for(unsigned i = low_edge; i < upper_edge; i++) {
        int x = i;
        
        int flag = 0;
        for(int j = 1; j < len; j++) {
            unsigned int r = A * x + C;
            r %= MOD;
            x = (int)r;
            if(pw[j] == '.'){
                continue;
            }
            b = x;
            double c = (double)b/(double)MOD;
            c *= 62.0;
            int d = (int)c;
            char ch;
            
            if(d <= 9){
                ch = '0' + d;
            }else if(d <= 35){
                ch = 'A' + d - 10;
            }else if(d <= 61) {
                ch = 'a' + d - 36;
            } else {
                ch = -1;
                return 1;
            }
            
            if(ch != pw[j]) {
                flag = 1;
                break;
            }
        }
        if(!flag) {
            goods++;
            nice_i = i;
        }
        if(goods > 1){
            break;
        }
    }
    if(goods == 1) {
        int x = nice_i;
        printf("%c", first_sym);
        for(int i = 1; i < len; i++) { 
            unsigned int r = A * x + C;
            r %= MOD;
            x = (int)r;
            b = x;
            double c = (double)b/(double)MOD;
            c *= 62.0;
            int d = (int)c;
            char ch;
            
            if(d <= 9){
                ch = '0' + d;
            }else if(d <= 35){
                ch = 'A' + d - 10;
            }else if(d <= 61) {
                ch = 'a' + d - 36;
            } else {
                ch = -1;
                return 1;
            }
            printf("%c", ch);
        }
        printf("\n");
    }else{
        printf("#\n");
    }
    
    return 0;
}
