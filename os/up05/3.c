#include <stdio.h>
#include <time.h>

enum
{
    BUFSIZE = 1000,
    YEAR_OFFSET = 1900,
    ARGNUM = 6
};

int main(int argc, char *argv[])
{
    FILE * file = fopen(argv[1], "r");
    if(file == NULL) {
        return 1;
    }
    
    int yyyy, mon, dd, hh, mm, ss;
    time_t last_time;
    int i = 0;
    while(fscanf(file, "%d/%d/%d %d:%d:%d", &yyyy, &mon, &dd, &hh, &mm, &ss) == ARGNUM) {
        struct tm time;
        time.tm_year = yyyy - YEAR_OFFSET;
        time.tm_mon = mon - 1;
        time.tm_mday = dd;
        time.tm_hour = hh;
        time.tm_min = mm;
        time.tm_sec = ss;
        time.tm_isdst = -1;
            
        time_t new_time = mktime(&time);
        if(i > 0) {
            printf("%ld\n", new_time - last_time);
        }
        last_time = new_time;
        i++;
    }
    
    fclose(file);
    return 0;
}
