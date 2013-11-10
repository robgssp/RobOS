#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

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

#endif
