#include <stdint.h>
#include <ioport.h>
#include <bits.h>
#include <memory.h>
#include <timer.h>

void init_pit() {
	out8(0x43, bit(2) | bit(4) | bit(5));
	out8(0x40, (1 << 8) - 1);
	out8(0x40, (1 << 8) - 1);
	out8(0x21, (1 << 8) - 1 - 1);
}