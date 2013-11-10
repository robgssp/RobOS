#include <stdint.h>
#include "sys.h"
#include "mmio.h"
#include "uart.h"
#include "moar.h"
#include "base64.h"

void uart_putptr(uint8_t *ptr1) {
        uint32_t ptr = (uint32_t) ptr1;
        uart_puts("0x");
        for (int32_t i = 7; i >= 0; --i) {
                int j = (ptr & (0xf << (4*i))) >> (4*i);
                if (j < 10) uart_putc(j+'0');
                else uart_putc(j-10+'A');
        }
}

void uart_putn(uint32_t n) {
        char str[12];
        str[11] = '\0';
        if (n == 0) {
                uart_putc('0');
                return;
        }
        int neg = n < 0;
        char *sp;
        for (sp = str+10; n != 0; sp--) {
                *sp = n % 10 + '0';
                n = n / 10;
        }
        if (neg) *(--sp) = '-';

        uart_puts(sp+1);
}

int quit() { return 1; }
int tits() {
        uart_puts("broke\r\n");
        return 0;
}

int charnum() {
        uart_putn(uart_getc());
        return 0;
}

// read in a line, then echo it
int parrot() {
        char *buf = moar(0);
        
        while ((*moar(1) = uart_getc()) != '\r');

        *moar(1) = '\r';
        *moar(1) = '\n';
        *moar(1) = '\0';
        uart_puts(buf);

        mat(buf);
        return 0;
}

uint8_t *b64thing;
// read in an executable, then run it
int store() {
        b64thing = moar(0);
        while ((*moar(1) = uart_getc()) != '\r') uart_putc(*moar(0));
        base64decode(b64thing, moar(0)-b64thing);
        return 0;
}

int read() {
        uart_puts(b64thing);
        mat(b64thing);
        return 0;
}

// A trivial command dispatch. If fn returns !=0, exit.
struct { char cmd; int (*fn)(); } funcs[] = {
        { 'q', quit },
        { 't', tits },
        { 'p', parrot },
        { 'c', charnum },
        { 'r', read },
        { 's', store }
};
const int nfuncs = sizeof(funcs) / sizeof(funcs[0]);

int main() {
        uart_init();
        moar_init();

        uart_puts(": RobOS :\r\n");

        while (1) {
                uart_puts(">");
                char c = uart_getc();
                int found = 0, ret;

                for (int i = 0; i < nfuncs; ++i) {
                        if (c == funcs[i].cmd) {
                                ret = funcs[i].fn();
                                found = 1;
                                break;
                        }
                }
                if (!found) {
                        uart_puts(": Not a command: ");
                        uart_putc(c);
                        uart_puts(" :\r\n");
                }
                if (ret) break;
        }

        uart_puts(": bye :");

        return 0;
}
