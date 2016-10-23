#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>
#include <io.h>
#include <memmap.h>

struct buddy {
    uint8_t lvl;
    char free;
};

void buddy_init(struct memory_map table[], size_t ptr);
uint64_t buddy_alloc(int level);
void buddy_free(uint64_t addr);

#endif