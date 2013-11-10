#ifndef UART_H
#define UART_H

#include <stdint.h>

enum {
        GPIO_BASE = 0x20200000,
        GPPUD = GPIO_BASE+0x94,
        GPPUDCLK0 = GPIO_BASE+0x98,

        UART = 0x20201000,
        UART_DR = UART+0x0,
        UART_RSRECR = UART+0x4,
        UART_FR = UART+0x18,
        UART_ILPR = UART+0x20,
        UART_IBRD = UART+0x24,
        UART_FBRD = UART+0x28,
        UART_LCRH = UART+0x2C,
        UART_CR = UART+0x30,
        UART_IFLS = UART+0x34,
        UART_IMSC = UART+0x38,
        UART_RIS = UART+0x3C,
        UART_MIS = UART+0x40,
        UART_ICR = UART+0x44,
        UART_DMACR = UART+0x48,
        UART_ITCR = UART+0x80,
        UART_ITIP = UART+0x84,
        UART_ITOP = UART+0x88,
        UART_TDR = UART+0x8C
};

void uart_init();
void uart_putc(uint8_t byte);
void uart_puts(char *bytes);
uint8_t uart_getc();

#endif
