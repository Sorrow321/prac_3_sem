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
    
    char alph[62];
    for(int i = 0; i < 62; i++) {
        if(i <= 9){
            alph[i] = '0' + i;
        }else if(i <= 35){
            alph[i] = 'A' + i - 10;
        }else{
            alph[i] = 'a' + i - 36;
        }
    }
    
    for(int i = 0; i < 62; i++){
        printf("%c", alph[i]);
    }
    scanf("%d", &len);
    
    char ans[MAX_LEN];
    ans[0] = first_sym;
    int goods = 0;
    for(unsigned i = low_edge; i < upper_edge; i++) {
        int x = i;
            
        int flag = 0;
        for(int j = 1; j < len; j++) {
            unsigned int r = A * x + C;
            r %= MOD;
            x = (int)r;
            b = x;
            double c = (double)b/(double)MOD;
            c *= 62.0;
            int d = (int)c;
            char ch = alph[d];
            
            
            if(pw[j] != '.' && ch != pw[j]) {
                flag = 1;
                break;
            }else{
                ans[j] = ch;
            }
        }
        if(!flag) {
            goods++;
        }
        if(goods > 1){
            break;
        }
    }
    ans[len] = 0;   
    if(goods == 1) {
        printf("%s\n", ans);
    }else{
        printf("#\n");
    }
    
    return 0;
}
