#include <stdio.h>

enum
{
    THREE_BITS_MOD = 256,
    THREE_BITS_ORD = 8
};

int main(int argc, char *argv[]){
    for(int i = 1; i < argc; i++) {
        char flags_template[] = "rwxrwxrwx";
        int a;
        sscanf(argv[i], "%o", &a);
		
        unsigned int mask = THREE_BITS_MOD;
        for(int j = 0; j < THREE_BITS_ORD + 1; j++) {
            if(!(a & mask)) {
                flags_template[j] = '-';
            }
            mask >>= 1;
        }
        printf("%s\n", flags_template);
    }
    return 0;
}
