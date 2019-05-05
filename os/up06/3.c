#include <string.h> 

int parse_rwx_permissions(const char *str)
{   
    if(str == NULL) {
        return -1;
    }
    const char str_templ[] = "rwxrwxrwx";
    unsigned int ans = 0;
    int len = strlen(str);
    const int len2 = sizeof(str_templ) - 1;
    if(len != len2) {
        return -1;
    }
    for(int i = 0; i < len; i++) {
        if(str[i] == str_templ[i]) {
            ans <<= 1;
            ans += 1;
        }else if(str[i] == '-'){
            ans <<= 1;
        }else{
            return -1;
        }
    }
    
    return ans;
}
