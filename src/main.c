static void qemu_gdb_hang(void)
{
#ifdef DEBUG
    static volatile int wait = 1;

    while (wait);
#endif
}

#include <desc.h>
#include <ints.h>
#include <serial.h>
#include <interrupt.h>
#include <timer.h>
#include <ioport.h>
#include <io.h>
#include <memmap.h>
#include <buddy.h>
#include <slab.h>
#include <threads.h>
#include <lock.h>

extern uint64_t handler[];

struct spinlock spinlock1;
struct spinlock spinlock2;

int val = 0;

void lock1(void * arg __attribute__((unused))) {
    printf("lock1\n");
    lock(&spinlock1);
    printf("in spinlock1 %d\n", val);
    yield();
    lock(&spinlock2);
    printf("in spinlock2 %d\n", val);
    val = 1;
    unlock(&spinlock2);
    printf("out of spinlock2 %d\n", val);
    unlock(&spinlock1);
    printf("out of spinlock1 %d\n", val);

}

void lock2(void * arg __attribute__((unused))) {
    printf("lock2\n");
    lock(&spinlock1);
    printf("in spinlock1 %d\n", val);
    lock(&spinlock2);
    printf("in spinlock2 %d\n", val);
    val = 2;
    unlock(&spinlock2);
    printf("out of spinlock2 %d\n", val);
    unlock(&spinlock1);
    printf("out of spinlock1 %d\n", val);
}
    

void main(void) {
    qemu_gdb_hang();
    disable_ints();
    init_serial();


    print_string("serial port");
    
    init_master_and_slave();
    init_idt();
    init_pit();

    enable_ints();
    
    //__asm__("int $40");

    disable_ints();

    get_memmap();
    print_memmap();

    init_threads();
    create_thread(lock1, 0);
    create_thread(lock2, 0);
    yield();
    yield();

    while (1);
}
