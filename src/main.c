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
	
	//__asm__("int $40");

	disable_ints();

	get_memmap();
	print_memmap();
	/*uint64_t mem = buddy_alloc(0);
	printf("%x\n", mem);
	buddy_free(mem);
	mem = buddy_alloc(1);
	printf("%x\n", mem);
	buddy_free(mem);
	mem = buddy_alloc(2);
	printf("%x\n", mem);
	buddy_free(mem);
	uint64_t mem1 = buddy_alloc(0);
	printf("%x\n", mem1);
	uint64_t mem2 = buddy_alloc(0);
	printf("%x\n", mem2);
	uint64_t mem3 = buddy_alloc(0);
	printf("%x\n", mem3);
	buddy_free(mem1);
	buddy_free(mem2);
	buddy_free(mem3);*/
	while (1);
}
