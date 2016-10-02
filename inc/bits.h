#ifndef __BITS_H__
#define __BITS_H__

#include <stdint.h>

#define bit(x) (1 << x)
#define byte0(x) ((x) & 0xffu)
#define byte1(x) (((x) >> 8) & 0xffu)


#endif