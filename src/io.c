#include <ioport.h>
#include <io.h>
#include <stdarg.h>
#include <stddef.h>
#include <serial.h>

void print_char(char c) {
    while (!(in8(serial_port + 5) & (1 << 5)));
    out8(serial_port, (uint8_t)c);
}


void print_c(char buf, char ** buffer, size_t sz, char c, size_t * num) {
    (*num)++;
    if (buf == 1) {
        if (*num >= sz) {
            (*num)--;
            return;
        }
        (**buffer) = c;
        (*buffer)++;
    }
    else {
        print_char(c);
    }
}


void print_number(char buf, char ** buffer, size_t sz, size_t * num, int64_t x, int base) {
    if (x < 0) {
        print_c(buf, buffer, sz, '-', num);
        x = -x;
    }
    char a[30];
    int i = 0;
    a[i] = 0;
    if (!x)
        a[i] = '0';
    while (x) {
        char need = x % base;
        if (need >= 10)
            need += 'a' - 10;
        else
            need += '0';
        a[i] = need;
        i++;
        x /= base;
    }
    if (i)
        i--;
    while (a[i] && i >= 0) {
        print_c(buf, buffer, sz, a[i], num);
        i--;
    }
}


int print(char buf, const char * fmt, char ** buffer, size_t sz, va_list arg) {
    size_t res = 0;
    while (*fmt) {
        char cur = *fmt;
        if (cur != '%') {
            print_c(buf, buffer, sz, cur, &res);
        } else {
            fmt++;
            cur = *fmt;
            if (cur == 'h') {
                fmt++;
                cur = *fmt;
                if (cur == 'd' || cur == 'i') {
                    short x = (short)va_arg(arg, int);
                    print_number(buf, buffer, sz, &res, x, 10);
                }
                else if (cur == 'u' || cur == 'o' || cur == 'x') {
                    int bs = 10;
                    if (cur == 'o')
                        bs = 8;
                    if (cur == 'x')
                        bs = 16;
                    unsigned short x = (unsigned short)va_arg(arg, unsigned int);
                    print_number(buf, buffer, sz, &res, x, bs);
                }
                else if (cur == 'h') {
                    fmt++;
                    cur = *fmt;
                    if (cur == 'd' || cur == 'i') {
                        char x = (char)va_arg(arg, int);
                        print_number(buf, buffer, sz, &res, x, 10);
                    }
                    else if (cur == 'u' || cur == 'o' || cur == 'x') {
                        int bs = 10;
                        if (cur == 'o')
                            bs = 8;
                        if (cur == 'x')
                            bs = 16;
                        unsigned char x = (unsigned char)va_arg(arg, unsigned int);
                        print_number(buf, buffer, sz, &res, x, bs);
                    }

                }
                
            }
            else if (cur == 'l') {
                fmt++;
                cur = *fmt;
                if (cur == 'd' || cur == 'i') {
                    long x = (long)va_arg(arg, long);
                    print_number(buf, buffer, sz, &res, x, 10);
                }
                else if (cur == 'u' || cur == 'o' || cur == 'x') {
                    int bs = 10;
                    if (cur == 'o')
                        bs = 8;
                    if (cur == 'x')
                        bs = 16;
                    unsigned long x = (unsigned long)va_arg(arg, unsigned long);
                    print_number(buf, buffer, sz, &res, x, bs);
                }
                else if (cur == 'l') {
                    fmt++;
                    cur = *fmt;
                    if (cur == 'd' || cur == 'i') {
                        long long x = (long long)va_arg(arg, long long);
                        print_number(buf, buffer, sz, &res, x, 10);
                    }
                    else if (cur == 'u' || cur == 'o' || cur == 'x') {
                        int bs = 10;
                        if (cur == 'o')
                            bs = 8;
                        if (cur == 'x')
                            bs = 16;
                        unsigned long long x = (unsigned long long)va_arg(arg, unsigned long long);
                        print_number(buf, buffer, sz, &res, x, bs);
                    }

                }
                
            }
            else if (cur == 'd' || cur == 'i') {
                int x = va_arg(arg, int);
                print_number(buf, buffer, sz, &res, x, 10);
            }
            else if (cur == 'u' || cur == 'o' || cur == 'x') {
                int bs = 10;
                if (cur == 'o')
                    bs = 8;
                if (cur == 'x')
                    bs = 16;
                unsigned int x = va_arg(arg, unsigned int);
                print_number(buf, buffer, sz, &res, x, bs);
            }
            else if (cur == 'c') {
                char c = va_arg(arg, int);
                print_c(buf, buffer, sz, c, &res);
            }
            else if (cur == 's') {
                char * x = va_arg(arg, char *);
                while (*x) {
                    print_c(buf, buffer, sz, *x, &res);
                    x++;
                }
            }
            else
                print_c(buf, buffer, sz, cur, &res);
        }
        fmt++;
    }
    if (buf)
        buffer[res] = 0;
    return res;
}


int vprintf(const char * fmt, va_list args) {
    return print(-1, fmt, 0, 0, args);
}

int printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = vprintf(fmt, args);
    va_end(args);
    return res;
}

int vsnprintf(char * buffer, size_t sz, const char * fmt, va_list args) {
    return print(1, fmt, &buffer, sz, args);
}

int snprintf(char * buffer, size_t sz, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = vsnprintf(buffer, sz, fmt, args);
    va_end(args);
    return res;
}
