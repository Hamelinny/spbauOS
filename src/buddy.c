#include <buddy.h>
#include <memmap.h>
#include <stdint.h>
#include <memory.h>

#define SIZE 25
#define SHIFT 0xffff800000000000

static struct buddy * ch;

static struct buddy * lists[SIZE];

uint32_t maxpages;

#define PAGE 4096
#define nil ((struct buddy *)1)

void add_buddy(int level, int num) {
    ch[num].next = lists[level];
    lists[level] = &ch[num];
}

uint64_t delete_buddy(int level) {
    uint64_t ans = (lists[level] - ch) * PAGE;
    lists[level] = lists[level]->next;
    return ans;
}
 
void buddy_init(struct memory_map table[], size_t ptr) {

    maxpages = 0;

    for (size_t i = 0; i < ptr; i++) {
        if (table[i].type == 1)
            maxpages = (table[i].base + table[i].len) / PAGE;
    }
    maxpages++;
    for (int i = 0; i < SIZE; i++)
        lists[i] = nil;
    uint32_t memory_to_use = maxpages * sizeof(struct buddy);
    for (size_t i = 0; i < ptr; i++) {
        if (table[i].type == 1) {
            if (table[i].len >= memory_to_use && table[i].base + table[i].len < (((uint64_t)1) << 32)) {
                ch = (struct buddy * )(table[i].base + SHIFT);
                printf("%x-%x %s\n", table[i].base, table[i].base + memory_to_use - 1, "for buddy allocator");
                table[i].base += memory_to_use;
                table[i].len -= memory_to_use;
                break;
            }
        }
    }

    for (size_t i = 0; i < maxpages; i++) {
        ch[i].free = 0;
        ch[i].usage = 0;
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
            ch[num].next = nil;
            ch[num].usage = 1;
            shift += PAGE;
        } 

    }
    for (int level = 1; level < SIZE; level++) {
        for (size_t i = 0; i < maxpages; i += (1 << level)) {
            size_t bud = i ^ (1 << (level - 1));
            if (bud >= maxpages)
                continue;
            if (ch[bud].usage == 0)
                continue;
            if (ch[i].lvl == level - 1 && ch[bud].lvl == level - 1 && ch[i].free == 1 && ch[bud].free == 1) {
                ch[i].lvl = level;
                ch[bud].free = 0;               
            }
        }
    }
   
    for (size_t i = 0; i < maxpages; i++) {
        if (ch[i].free == 1 && ch[i].usage == 1) {
            add_buddy(ch[i].lvl, i);           
        }
    }
}

void prepare(int level) {
    if (level + 1 == SIZE)
        return;
    if (lists[level] != nil)
        return;
    prepare(level + 1);
    if (lists[level + 1] == nil) {
        printf("Sorry, no memory\n");
        while (1);
    }
    size_t num = delete_buddy(level + 1) / PAGE;
    ch[num].lvl = level;
    if ((num ^ (1 << level)) < maxpages) {
        ch[num ^ (1 << level)].lvl = level;
        ch[num ^ (1 << level)].free = 1;
        add_buddy(level, num ^ (1 << level)); 
    }
    add_buddy(level, num);
    
}


uint64_t buddy_alloc(int level) {
    prepare(level);
    return delete_buddy(level) + SHIFT;
}

void merge(size_t num) {
    int level = ch[num].lvl;
    if (level + 1 == SIZE || (num ^ (1 << level)) >= maxpages || ch[num ^ (1 << level)].usage == 0 || ch[num ^ (1 << level)].free == 0 || ch[num ^ (1 << level)].lvl != level) {
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
    size_t num = (addr - SHIFT) / PAGE;
    ch[num].free = 1;
    merge(num);
}
