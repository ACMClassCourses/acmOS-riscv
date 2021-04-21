//
// low-level driver routines for 16550a UART.
//

#include "defs.h"
#include "memlayout.h"
#include "riscv.h"
#include "lock.h"
#include "uart.h"


// clear the screen
void uart_cls(){
    uart_putc(12);
    uart_putc(27);
    uart_putc('[');
    uart_putc('2');
    uart_putc('J');
}

void uart_init(void) {
    // disable interrupts.
    WriteReg(IER, 0x00);

    // special mode to set baud rate.
    WriteReg(LCR, 0x80);

    // LSB for baud rate of 38.4K.
    WriteReg(0, 0x03);

    // MSB for baud rate of 38.4K.
    WriteReg(1, 0x00);

    // leave set-baud mode,
    // and set word length to 8 bits, no parity.
    WriteReg(LCR, 0x03);

    // reset and enable FIFOs.
    WriteReg(FCR, 0x07);

    // enable receive interrupts.
    WriteReg(IER, 0x01);

    uart_cls();
}


// write one output character to the UART.
void uart_putc(int c) {
    // wait for Transmit Holding Empty to be set in LSR.
    while ((ReadReg(LSR) & (1 << 5)) == 0);
    WriteReg(THR, c);
}

// read one input character from the UART.
// return -1 if none is waiting.
int uart_getc(void) {
    if (ReadReg(LSR) & 0x01) {
        // input data is ready.
        return ReadReg(RHR);
    } else {
        return -1;
    }
}

// trap.c calls here when the uart interrupts.
//void uart_intr(void) {
//    while (1) {
//        int c = uart_getc();
//        if (c == -1) break;
//        consoleintr(c);
//    }
//}
