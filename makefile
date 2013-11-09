ARMGNU ?= arm-none-eabi

SOURCES_ASM := $(wildcard *.S)
SOURCES_C   := $(wildcard *.c)

OBJS := $(patsubst %.S,%.o,$(SOURCES_ASM))
OBJS += $(patsubst %.c,%.o,$(SOURCES_C))

ASFLAGS := -D__ASSEMBLY__
CFLAGS  := -ffreestanding -std=gnu99
# CFLAGS  += -O2 -fpic -pedantic -pedantic-errors -nostdlib
# CFLAGS  += -nostartfiles -ffreestanding -nodefaultlibs
# CFLAGS  += -fno-builtin -fomit-frame-pointer -mcpu=arm1176jzf-s

.PHONY: clean

kernel.elf: $(OBJS) link-arm-eabi.ld
	$(ARMGNU)-ld $(OBJS) -Tlink-arm-eabi.ld -o $@

.c.o:
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

.S.o:
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

clean:
	rm kernel.elf *.o || true
