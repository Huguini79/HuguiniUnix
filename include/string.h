#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str);
int strncmp(const char* str1, const char* str2, int n);

#endif