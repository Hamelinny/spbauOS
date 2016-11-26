#include "lock.h"
#include "threads.h"

void lock() {
    if (get_cnt_lock() == 0)
        __asm__ volatile ("cli");
    update_cnt_lock(1);
}

void unlock() {
    update_cnt_lock(-1);
    if (get_cnt_lock() == 0)
        __asm__ volatile ("sti");
}
