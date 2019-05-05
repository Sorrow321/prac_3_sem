#include <stdio.h>
#include <stdlib.h>

enum
{
    START_SIZE = 128,
    SEPARATOR = 0x20,
    MASK = 192,
    MASKK = 128
};

int main(void)
{
    char *max_word = NULL;
    unsigned max_word_len = 0;
    int a = 0;
    unsigned capacity = START_SIZE;
    unsigned size = 0;
    unsigned max_size = 0;
    while((a = getchar_unlocked()) != EOF) {
        if(a > SEPARATOR) {
            // началось новое слово
            capacity = START_SIZE;
            size = 1;
            unsigned word_len = 1;
            char *current_word = calloc(capacity, sizeof(*current_word));
            if(!current_word) {
                break;
            }
            current_word[0] = a;
            int b = 0;
            while((b = getchar_unlocked()) != EOF && b > SEPARATOR) {
                current_word[size++] = b;
                if (((unsigned)b & MASK)!= MASKK) {
                    word_len++;
                }
                if(capacity - size < 2) {
                    capacity *= 2;
                    char *new_arr = realloc(current_word, capacity * sizeof(*new_arr));
                    if (new_arr == NULL) {
                        free(current_word);
                        break;
                    }
                    current_word = new_arr;
                }
            }
            if(word_len > max_word_len) {
                free(max_word);
                max_word = current_word;
                max_word_len = word_len;
                max_size = size;
            }else{
                free(current_word);
            }
        }
    }
    
    printf("%u\n", max_word_len);
    if(max_word_len > 0) {
        max_word[max_size] = 0;
        printf("%s\n", max_word);
    }
    if(max_word) {
        free(max_word);
    }
    return 0;
}
