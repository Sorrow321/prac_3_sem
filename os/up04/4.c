void normalize_path(char *buf)
{
    char *p1 = buf, *p2 = buf, slash_num = 0;
    while(*p1) {
        if(*p1 == '/') {
            slash_num = (slash_num == 2)? 2 : slash_num + 1;
        }else{
            slash_num = 0;
        }
        if(slash_num <= 1) {
            *p2 = *p1;
            p2++;
        }
        p1++;
    }
    *p2 = 0;
}
