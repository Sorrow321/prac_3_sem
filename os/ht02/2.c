#include <stdio.h>
#include <math.h>

enum
{
    FOUR_DIGITS = 10000 // = 10^4, т.к. работаем в десятичной СС.
};

const double FULL_PART = 100.0; // 100%

int main(int argc, char *argv[])
{
    double base;
    if(!sscanf(argv[1], "%lf", &base)){
        return 1;
    }

    for(int i = 2; i < argc; i++) {
        double change;
        if(!sscanf(argv[i], "%lf", &change)) {
            return 1;
        }
        base += base * change / FULL_PART;
        base = round(base * FOUR_DIGITS) / FOUR_DIGITS; // Перенос знака на 4 позиции, округление до целого и возврат знака
    }

    printf("%.4f\n", base);

    return 0;
}
