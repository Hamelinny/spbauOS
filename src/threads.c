#include "interrupt.h"
#include "threads.h"
#include "buddy.h"
#include "assert.h"


#define CREATE 0
#define RUN 1
#define FINISH 2


struct thread_data  {
    uint64_t r15, r14, r13, r12, rbx, rbp;
    void * start_addr;
    void (* fun_addr )(void *);
    void * arg;
};

struct thread {
    void * stack_pointer;
    void * stack_start;
    int sz;
    int state;
};

static volatile pid_t current_thread = 1;
extern void * start_thread;
volatile struct thread threads[100];
uint8_t size = 2;

pid_t get_cur_thread() {
    return current_thread;
}

void init_threads() {
    threads[1].state = RUN;
    threads[0].state = CREATE;
}

pid_t create_thread(void (* fun)(void *), void * arg) {
    __asm__ volatile ("cli");
    volatile struct thread * new_thread = &threads[__sync_fetch_and_add(&size, 1)];
    new_thread->sz = 4;
    new_thread->stack_start = (void *)buddy_alloc(new_thread->sz);
    new_thread->stack_pointer = (uint8_t *)new_thread->stack_start + 4096 * (1 << (new_thread->sz)) - sizeof(struct thread_data);

    struct thread_data * init = new_thread->stack_pointer;
    init->r12 = 0;
    init->r13 = 0;
    init->r14 = 0;
    init->r15 = 0;
    init->rbx = 0;
    init->rbp = 0;
    init->start_addr = &start_thread;
    init->fun_addr = fun;
    init->arg = arg;
    new_thread->state = RUN;
    __asm__ volatile ("sti");
    return (pid_t)(new_thread - threads);
}

void run(pid_t id) {
    if (current_thread == id) {
        return;
    }
    struct thread * thread = (struct thread *)threads + id;
    pid_t prev = current_thread;
    current_thread = id;
    struct thread * pthread = (struct thread *)threads + prev;
    pid_t pr = switch_threads(&pthread->stack_pointer, thread->stack_pointer);
    free_thread(pr);
}

void yield() {
    __asm__ volatile ("cli");
    pid_t i = (current_thread + 1) % size;
    while (1) {
        if (i == 0 || threads[i].state != RUN) {
            i = (i + 1) % size;
            continue;
        } 
        run(i);
        break;

    }
    __asm__ volatile ("sti");
}

void finish_thread() {
    volatile struct thread * cur = (struct thread *)threads + current_thread;
    cur->state = FINISH;
    yield();
}

void free_thread(pid_t prev_thread) {
    volatile struct thread * thread = (struct thread *)threads + prev_thread;
    if (thread->state == FINISH && prev_thread != current_thread) {
        buddy_free((uint64_t)thread->stack_start);
        thread->state = CREATE;
    }
}




