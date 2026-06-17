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

/* COPIED FROM OSDEV */
char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}