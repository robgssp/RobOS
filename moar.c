// Memory management the Best Way
// Allocate with moar, dealloc everything back to a place
// with mat.

#include "moar.h"

extern uint8_t *_end;
static uint8_t *memend;

void moar_init() {
        memend = _end;
}

uint8_t *moar(int32_t bytes) {
        uint8_t *ret = memend;
        memend += bytes;
        return ret;
}

void mat(uint8_t *place) {
        memend = place;
}
