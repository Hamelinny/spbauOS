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

extern uint64_t handler[];

void main(void)
{
	qemu_gdb_hang();
	disable_ints();
	init_serial();


	print_string("serial port");
	
	init_master_and_slave();
	init_idt();
	init_pit();

    enable_ints();
	
	__asm__("int $40");

	
	while (1);
	out8(0x21, (1 << 8) - 1);
}
