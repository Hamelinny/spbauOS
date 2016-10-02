#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>

struct dsc_idt {
	uint16_t offset0_15;
	uint16_t segment_selector;
	uint8_t ist;
	uint8_t p_dpl_type;
	uint16_t offset16_31;
	uint32_t offset32_63;
	uint32_t reserved;
} __attribute__ ((packed));

void init_idt();
void init_master_and_slave();


#endif