#include <memmap.h>
#include <stdint.h>
#include <io.h>
#include <buddy.h>

extern uint64_t multiboot_info;
extern uint32_t multiboot_header[];
uint32_t size_of = 20;
struct memory_map table[128];
size_t ptr = 0;

void get_memmap(void) {
	if ((!((*(uint32_t *)(multiboot_info)) & (1 << 6)))) {
		printf("%s\n", "No memmap");
		return;
	}
	struct memory_map kernel;
	kernel.base = multiboot_header[4];
	kernel.len = multiboot_header[6] - multiboot_header[4] + 1;
	kernel.type = 0;
	kernel.size = size_of;
	
	table[ptr] = kernel;
	ptr++;

	uint32_t mlen = *(uint32_t *)(multiboot_info + 44);
    uint32_t maddr = *(uint32_t *)(multiboot_info + 48);
    uint32_t cur = 0;
    while (cur < mlen) {
    	struct memory_map entry;
    	entry.size = *(uint32_t *)(uint64_t)(maddr + cur);
    	uint32_t sz = entry.size;
        entry.base = *(uint64_t *)(uint64_t)(maddr + cur + 4);
        entry.len = *(uint64_t *)(uint64_t)(maddr + cur + 12);
        entry.type = *(uint32_t *)(uint64_t)(maddr + cur + 20);
        if (kernel.base + kernel.len - 1 < entry.base || entry.base + entry.len - 1 < kernel.base) {
			table[ptr++] = entry;
		} else if (kernel.base <= entry.base && entry.base + entry.len - 1 <= kernel.base + kernel.len - 1) {
			//nothing to add
		} else if (entry.base < kernel.base && kernel.base + kernel.len - 1 < entry.base + entry.len - 1) {
			struct memory_map entry2 = entry;
			entry.len = kernel.base - entry.base;
			entry2.base = kernel.base + kernel.len;
			entry2.len -= kernel.len + entry.len;
			table[ptr++] = entry;
			table[ptr++] = entry2;
		} else if (kernel.base < entry.base) {
			entry.base = kernel.base + kernel.len;
			table[ptr++] = entry;
		} else {
			entry.len = kernel.base - entry.base;	
		}
		cur += sz + 4;
    }

    for (size_t i = 0; i < ptr - 1; i++) {
		for (size_t j = 0; j < ptr - i - 1; j++) {
			if (table[j].base > table[j + 1].base) {
				struct memory_map tmp = table[j];
				table[j] = table[j + 1];
				table[j + 1] = tmp;
			}
		}
	}
	buddy_init(table, ptr);
}


void print_memmap(void) {
	printf("%s\n", "Memmap:");
	for (size_t i = 0; i < ptr; i++) {

		printf("%x-%x %d\n", table[i].base, table[i].base + table[i].len - 1, table[i].type);
	}
}