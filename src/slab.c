#include <slab.h>
#include <buddy.h>
#include <io.h>

#define ALLOC_SIZE 16384
#define nil (struct slab_allocator *)(-1)

struct slab_allocator * init_slab(size_t size) {
	if (size < sizeof(struct slab_allocator))
		size = sizeof(struct slab_allocator);
	struct slab_allocator * alloc = (struct slab_allocator *)buddy_alloc(2);
	alloc->size = size;
	struct slab_allocator * cur = alloc;
	size_t i = 2;
	while (i * size < ALLOC_SIZE) {
		cur->next = (struct slab_allocator *)((char *)(alloc) + (i - 1) * size);
		i++;
		cur = cur->next;
		cur->size = size;
	}
	cur->next = nil;
	return alloc;
}

void * alloc_slab(struct slab_allocator ** slab) {
	if ((*slab)->next == nil) {
		struct slab_allocator * new_tail = init_slab((*slab)->size);
		(*slab)->next = new_tail;
	}
	void * ans = (void *)(*slab);
	(*slab) = (*slab)->next;
	return ans;
}

void free_slab(struct slab_allocator ** slab, void * mem) {
	struct slab_allocator * new_head = (struct slab_allocator *)mem;
	new_head->size = (*slab)->size;
	new_head->next = (*slab);
	(*slab) = new_head;
}