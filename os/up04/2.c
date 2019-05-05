#include <stdio.h>
#include <string.h>

const char flags_template[] = "rwxrwxrwx";

enum
{
    TEMP_LEN = sizeof(flags_template) - 1
};

int main(int argc, char *argv[]){
    for(int i = 1; i < argc; i++) {
        int a;
        sscanf(argv[i], "%o", &a);
		
        char len = TEMP_LEN - 1;
        unsigned int mask = 1 << len;
        for(int j = 0; j < len + 1; j++) {
            if(!(a & mask)) {
                printf("-");
            }else{
                printf("%c", flags_template[j]);
            }
            mask >>= 1;
        }
        printf("\n");
    }
    return 0;
}
