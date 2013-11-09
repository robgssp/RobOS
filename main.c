#include <stdint.h>

enum {
        GPIO_BASE = 0x20200000,
        GPPUD = GPIO_BASE+0x94,
        GPPUDCLK0 = GPIO_BASE+0x98,

        UART = 0x101f1000,
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


// write to mmio
static inline void mmio_write(uint32_t reg, uint32_t data) {
        uint32_t *ptr = (uint32_t*)reg;
        asm volatile("str %[data], [%[reg]]"
                     : : [reg]"r"(ptr), [data]"r"(data));
}

// read from mmio
static inline uint32_t mmio_read(uint32_t reg) {
        uint32_t *ptr = (uint32_t*)reg;
        uint32_t data;
        asm volatile("ldr %[data], [%[reg]]"
                     : [data]"=r"(data) : [reg]"r"(ptr));
        return data;
}

static void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
	     : : [count]"r"(count) : "cc");
}

void uart_init() {
    // Disable UART.
    mmio_write(UART_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15.
 
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    delay(150);
 
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
 
    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);
 
    // Clear pending interrupts.
    mmio_write(UART_ICR, 0x7FF);
 
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
 
    // Divider = 3000000/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART_IBRD, 1);
    mmio_write(UART_FBRD, 40);
 
    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
    // Mask all interrupts.
    mmio_write(UART_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
		    (1 << 6) | (1 << 7) | (1 << 8) |
		    (1 << 9) | (1 << 10));
 
    // Enable UART, receive & transfer part of UART.
    mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(uint8_t byte) {
        while ((mmio_read(UART_FR) & (1 << 5)));
        mmio_write(UART_DR, byte);
}

void uart_puts(char *bytes) {
        while(*bytes) uart_putc(*(bytes++));
}

void uart_putptr(uint32_t ptr) {
        uart_puts("0x");
        for (int i = 8; i > 0; --i) {
                uart_putc((ptr & (0xa << (4*i))) >> (4*i));
        }
}

int main() {
        uart_init();
        //uart_puts("tits.\n");
        uart_putc('T');
        //uart_putptr((int)&main);
        return 0;
}
