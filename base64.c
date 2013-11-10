#include "base64.h"
#include "uart.h"
#include "moar.h"

int base64dchar(int ch) {
        if (ch >= 'A' && ch <= 'Z') {
                return ch - 'A';
        } else if (ch >= 'a' && ch <= 'z') {
                return ch - 'a' + 26;
        } else if (ch >= '0' && ch <= '9') {
                return ch - '0' + 52;
        } else if (ch == '+') {
                return 62;
        } else if (ch == '-') {
                return 63;
        } else {
                return 0;
        }
}

/* warning: in-place conversion */
void base64decode(char *in, int inlen) {
        int outlen = (inlen * 3) / 4;
        char *out = in;
        for (int i = 0; i < inlen; i += 4) {
                uint32_t b = (base64dchar(in[i]) << 18) |
                        (base64dchar(in[i+1]) << 12) |
                        (base64dchar(in[i+2]) << 6) |
                        (base64dchar(in[i+3]));
                *out++ = b >> 16;
                *out++ = b >> 8;
                *out++ = b;
        }
        *out++ = '\0';
}
