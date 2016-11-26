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

    while (1);
}
