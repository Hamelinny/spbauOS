#include <buddy.h>
#include <memmap.h>
#include <stdint.h>

#define SIZE 18

static struct buddy ch[1 << SIZE];

static uint64_t lists[SIZE][1 << SIZE];

static uint32_t ptrs[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define PAGE 4096

void add_buddy(int level, int num) {
	lists[level][ptrs[level]++] = num;
}

uint64_t delete_buddy(int level) {
	return lists[level][--ptrs[level]];
}
 
void buddy_init(struct memory_map table[], size_t ptr) {
	for (size_t i = 0; i < (1 << SIZE); i++) {
		ch[i].free = 0;
	}
    for (size_t i = 0; i < ptr; i++) {
    	if (table[i].type != 1)
    		continue;
    	printf("%x-%x %s\n", table[i].base, table[i].base + table[i].len - 1, "free memory");
        uint64_t pointer = table[i].base;
        uint64_t shift = 0;
        if (pointer % PAGE)
        	shift += PAGE - (pointer % PAGE);
        while (shift + PAGE <= table[i].len) {
            int num = (pointer + shift) / PAGE;
            ch[num].lvl = 0; 
            ch[num].free = 1;
            shift += PAGE;
        } 

    }
    for (int level = 1; level < SIZE; level++) {
        for (int i = 0; i < (1 << SIZE); i += (1 << level)) {
            int bud = i ^ (1 << (level - 1));
            if (ch[i].lvl == level - 1 && ch[bud].lvl == level - 1 && ch[i].free == 1 && ch[bud].free == 1) {
                ch[i].lvl = level;
                ch[bud].free = 0;               
            }
        }
    }
   
    for (int i = 0; i < (1 << SIZE); i++) {
        if (ch[i].free == 1) {
            add_buddy(ch[i].lvl, i);           
        }
    }
}

void prepare(int level) {
	if (level + 1 == SIZE)
		return;
	if (ptrs[level] != 0)
		return;
	prepare(level + 1);
	if (ptrs[level + 1] == 0) {
		printf("Sorry, no memory\n");
		while (1);
	}
	size_t num = lists[level + 1][--ptrs[level + 1]];
    ch[num].lvl = level;
    ch[num ^ (1 << level)].lvl = level;
    ch[num ^ (1 << level)].free = 1;
    add_buddy(level, num ^ (1 << level)); 
    add_buddy(level, num);
    
}


uint64_t buddy_alloc(int level) {
	prepare(level);
	ch[lists[level][--ptrs[level]]].free = 0;
	return lists[level][ptrs[level]] * PAGE;
}

void merge(size_t num) {
	int level = ch[num].lvl;
	if (level + 1 == SIZE || ch[num ^ (1 << level)].free == 0 || ch[num ^ (1 << level)].lvl != level) {
		add_buddy(level, num);
		return;
	}
	if ((num ^ (1 << level)) < num) {
		num = num ^ (1 << level);
	}
	
	ch[num ^ (1 << level)].free = 0;
	ch[num].lvl++;
	merge(num);
}

void buddy_free(uint64_t addr) {
	size_t num = addr / PAGE;
	ch[num].free = 1;
	merge(num);
}