.section ".text.boot"
.globl _start
        
_start:
        // zero the BSS
        mov     r5, #0
        mov     r6, #0
        mov     r7, #0
        mov     r8, #0

        ldr     r4, =_bss_start
        ldr     r9, =_bss_end
        b       zcmp
zstart:
        stm     r4!, { r5,r6,r7,r8 }
        cmp     r4, r9
zcmp:   
        blo     zstart
        mov     r0, #'S'
        blx     uart_putc

        blx     main
sleep:  
        wfe
        b       sleep
