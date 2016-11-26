#include "lock.h"
#include "threads.h"
#include "ints.h"
#include <stdatomic.h>


int cnt = 0;

void lock() {
    disable_ints();
    cnt++;
}

void unlock() {
    cnt--;
    if (cnt == 0)
        enable_ints();
}
