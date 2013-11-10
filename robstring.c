#include "robstring.h"

int strcmp(char *str1, char *str2) {
        if (str1 == str2) return 0;
        while (*str1 == *str2) {
                if (*str1 == '\0') return 0;
                str1++; str2++;
        }
        return 1;
}

