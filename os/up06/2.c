#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

enum
{
    ARGS_NUM = 3,
    CONST_YEAR = 1900,
    FIRST_YEAR = 1925,
    FIRST_MONTH = 10,
    FIRST_DAY = 7,
    HOURS_IN_DAY = 24,
    SEC_IN_MIN = 60,
    MIN_IN_HOUR = 60,
    DAYS_IN_MONTH = 30,
    MONTHS_IN_YEAR = 12
};

int main(void)
{
    int yy, mm, dd;
    
    struct tm atime;
    memset(&atime, 0, sizeof(atime));
    atime.tm_year = FIRST_YEAR - CONST_YEAR;
    atime.tm_mon = FIRST_MONTH - 1;
    atime.tm_mday = FIRST_DAY;
    
    const long long first_time = timegm(&atime);
    while((scanf("%d%d%d", &yy, &mm, &dd) == ARGS_NUM)) {
        struct tm btime;
        memset(&btime, 0, sizeof(btime));
        btime.tm_year = yy - CONST_YEAR;
        btime.tm_mon = mm - 1;
        btime.tm_mday = dd;
        btime.tm_isdst = -1;
        long long cur_time = timegm(&btime);
        long long new_time = cur_time - first_time; // Разница в секундах между поданным временем и началом нового календаря  
        
        int day, mon, year;
        new_time /= HOURS_IN_DAY * MIN_IN_HOUR * SEC_IN_MIN;
        day = new_time % DAYS_IN_MONTH; // Находим номер дня
        
        new_time /= DAYS_IN_MONTH;
        mon = new_time % MONTHS_IN_YEAR; // Номер месяца
        
        new_time /= MONTHS_IN_YEAR;
        year = new_time; // Номер года
        
        printf("%d %d %d\n", year + 1, mon + 1, day + 1);
    }
    return 0;
}
