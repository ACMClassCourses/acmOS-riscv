//
// Created by Wenxin Zheng on 2021/1/9.
//

#ifndef ACMOS_SPR21_UART_H
#define ACMOS_SPR21_UART_H

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
#define Reg(reg) ((volatile unsigned char *)(UART0 + reg))

// the UART control registers.
// some have different meanings for
// read vs write.
// http://byterunner.com/16550.html
#define RHR 0  // receive holding register (for input bytes)
#define THR 0  // transmit holding register (for output bytes)
#define IER 1  // interrupt enable register
#define FCR 2  // FIFO control register
#define ISR 2  // interrupt status register
#define LCR 3  // line control register
#define LSR 5  // line status register

#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

void uart_init(void);
void uart_putc(int c);
int uart_getc(void);

#endif  // ACMOS_SPR21_UART_H
