#ifndef __THREADS__
#define __THREADS__

#include <sys/types.h>


pid_t get_current_thread();

void init_threads();

pid_t create_thread(void (*fun)(void *), void *arg);

void run(pid_t tid);

void yield();

void finish_thread();

void free_thread(pid_t prev_thread);

int get_cnt_lock();

void update_cnt_lock(int val);


pid_t switch_threads(void ** old_sp, void * new_sp);


#endif
