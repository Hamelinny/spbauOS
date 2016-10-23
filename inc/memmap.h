#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include <stdint.h>

struct memory_map {
    uint32_t size;
    uint64_t base;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));


void get_memmap(void);

void print_memmap(void);

#endif
