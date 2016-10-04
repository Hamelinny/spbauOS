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

	printf("%d\n", 10);
	printf("%ho\n", 8);
	printf("%hhx\n", 16);
	printf("%x\n", 255);
	char a[10];
	char * x = a;
	printf("%s\n", "hello");
	printf("%d\n", snprintf(x, 10, "%s", "hello"));
	printf("%s\n", a);
	printf("%lld\n", (1ll << 62));
	
	while (1);
	out8(0x21, (1 << 8) - 1);
}
