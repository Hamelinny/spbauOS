#include <stdint.h>
#include <serial.h>
#include <ioport.h>
#include <bits.h>

const int serial_port = 0x3f8;

void print_string(const char *s) {
    int i = 0;
    while (s[i]) {
        while (!(in8(serial_port + 5) & (1 << 5)));
        out8(serial_port, (uint8_t)s[i]);
        i++;
    }
    
}

void init_serial() {

    out8(serial_port + 3, bit(7));
    uint16_t div = 1;
    out8(serial_port, byte0(div));
    out8(serial_port + 1, byte1(div));
    
    out8(serial_port + 3, bit(0) | bit(1));

    out8(serial_port + 1, 0);
}

