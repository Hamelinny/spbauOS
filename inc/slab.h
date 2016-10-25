#ifndef __SLAB_H__
#define __SLAB_H__

#include <io.h>

struct slab_allocator {
	size_t size;
	struct slab_allocator * next;
};

struct slab_allocator * init_slab(size_t size);

void * alloc_slab(struct slab_allocator ** slab);

void free_slab(struct slab_allocator ** slab, void * mem) ;

#endif