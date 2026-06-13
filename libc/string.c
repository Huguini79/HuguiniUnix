#include "include/string.h"
#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str)
{
    size_t len = 0;

    while (str[len] != '\0')
    {
        len++;
    }

    return len;

}

int strncmp(const char* str1, const char* str2, int n)
{
    unsigned char u1, u2;
    for (int i = 0; i < n; ++i)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2)
        {
            return u2 - u1;
        }

        if (u1 == '\n')
        {
            return 0;
        }

    }

    return 0;
}