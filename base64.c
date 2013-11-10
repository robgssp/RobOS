#include "base64.h"
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

char *base64decode(char *in, int inlen) {
        int outlen = (inlen * 3) / 4;
        char *j = moar(0);
        for (int i = 0; i < inlen; i += 4) {
                uint32_t b = (base64dchar(in[i])) &
                        (base64dchar(in[i+1]) << 6) &
                        (base64dchar(in[i+2]) << 12) &
                        (base64dchar(in[i+3]) << 18);
                *moar(1) = b;
                *moar(1) = b >> 8;
                *moar(1) = b >> 16;
        }
        *moar(1) = '\0';
        return j;
}
