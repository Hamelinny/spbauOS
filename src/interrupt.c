#include <stdint.h>
#include <ioport.h>
#include <desc.h>
#include <serial.h>
#include <interrupt.h>
#include <bits.h>
#include <memory.h>

struct dsc_idt idt[256];
extern uint64_t handler[];

void init_idt() {

    for (int i = 0; i < 256; i++) 
    {
        idt[i].segment_selector = KERNEL_CS;
        idt[i].offset0_15 = handler[i] & ((1 << 16) - 1);
        idt[i].offset16_31 = (handler[i] >> 16) & ((1 << 16) - 1);
        idt[i].offset32_63 = (handler[i] >> 32);
        idt[i].ist = 0;
        idt[i].reserved = 0;
        idt[i].p_dpl_type = 14 + (1 << 7);
    }

    struct desc_table_ptr pointer = {sizeof(idt) - 1, (uint64_t)&idt};
    write_idtr(&pointer);
}

void interrupt_handler(uint64_t id)
{
    print_string("Interrupt!\n");
    char a[2];
    a[1] = 0;
    a[0] = '0' + id / 10;
    print_string(a);
    a[0] = '0' + id % 10;
    print_string(a);
    print_string("\n");
    if (id >= 32 && id < 40) {
      out8(0x20, bit(5));
    }
    else if (id >= 40 && id < 48) {
      out8(0xA0, bit(5));
      out8(0x20, bit(5));
    }
}

void init_master_and_slave() {
    out8(0x20, bit(4) | bit(0));
    out8(0x21, 32);
    out8(0x21, 4);
    out8(0x21, 1);
    out8(0x21, (1 << 8) - 1);
    out8(0xA0, bit(4) | bit(0));
    out8(0xA1, 40);
    out8(0xA1, 2);
    out8(0xA1, 1);
    out8(0xA1, (1 << 8) - 1);
}