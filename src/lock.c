#include "lock.h"
#include "threads.h"
#include "ints.h"

void lock() {
    if (get_cnt_lock() == 0)
        disable_ints();
    update_cnt_lock(1);
}

void unlock() {
    update_cnt_lock(-1);
    if (get_cnt_lock() == 0)
        enable_ints();
}
