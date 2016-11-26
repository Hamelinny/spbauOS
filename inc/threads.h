#ifndef __THREADS__
#define __THREADS__


int get_current_thread();

void init_threads();

int create_thread(void (*fun)(void *), void *arg);

void run(int tid);

void yield();

void finish_thread();

void free_thread(int prev_thread);



int switch_threads(void ** old_sp, void * new_sp);


#endif
