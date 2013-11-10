ARMGNU ?= arm-none-eabi

SOURCES_ASM := $(wildcard *.S)
SOURCES_C   := $(wildcard *.c)

OBJS := $(patsubst %.S,%.o,$(SOURCES_ASM))
OBJS += $(patsubst %.c,%.o,$(SOURCES_C))

BASEFLAGS := -mcpu=arm1176jzf-s -I.
ASFLAGS   := $(BASEFLAGS) -D__ASSEMBLY__
CFLAGS    := $(BASEFLAGS) -ffreestanding -std=gnu99 -Wno-implicit-int

.PHONY: clean emu

kernel.elf: $(OBJS) link-arm-eabi.ld
	$(ARMGNU)-ld -static $(OBJS) -Tlink-arm-eabi.ld -o $@

.c.o:
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

.S.o:
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

clean:
	rm kernel.elf *.o || true

emu: kernel.elf
	 ~/build/qemu/qemu-rpi-build/arm-softmmu/qemu-system-arm -kernel kernel.elf -cpu arm1176 -m 256 -M raspi -nographic -monitor none -serial stdio
