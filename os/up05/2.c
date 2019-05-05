#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum
{
    CORRECT_ARGNUM = 3,
    YEAR_OFFSET = 1900,
    HOURS_IN_DAY = 24,
    MIN_IN_HOUR = 60,
    SEC_IN_MIN = 60
};

int main(int argc, char *argv[])
{
    if(argc < CORRECT_ARGNUM) {
        return 1;
    }
    int year, day;
    sscanf(argv[1], "%d", &year);
    sscanf(argv[2], "%d", &day);
    
    struct tm datestr = {};
    datestr.tm_year =  year - YEAR_OFFSET;
    datestr.tm_isdst = -1;
	
    time_t utime = mktime(&datestr) + day * HOURS_IN_DAY * MIN_IN_HOUR * SEC_IN_MIN;
    if(utime == (time_t) -1) {
        return 1;
    }
    
    localtime_r(&utime, &datestr);
    printf("%d %d %d %d\n", datestr.tm_year + YEAR_OFFSET, datestr.tm_mon + 1, 
        datestr.tm_mday, datestr.tm_wday? datestr.tm_wday : 7);
    
    return 0;
}
