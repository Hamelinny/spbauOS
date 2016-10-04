#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

#define serial_port 0x3f8
void print_string(const char *);
void init_serial();


#endif
