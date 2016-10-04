#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

void print_char(char);

void print_c(char, char **, size_t, char, size_t *);

void print_unsigned(char, char **, size_t, size_t *, uint64_t, int);

void print_int(char, char **, size_t, size_t *, int64_t, int);

int print(char, const char *, char **, size_t, va_list);

int vprintf(const char *, va_list);
int printf(const char *, ...);

int vsnprintf(char *, size_t, const char *, va_list);

int snprintf(char *, size_t, const char *, ...);

#endif
