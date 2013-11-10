.section ".text.boot"
.globl _start
        
_start:
        mov      sp, #0x8000
        
        // zero the BSS
        mov     r5, #0
        mov     r6, #0
        mov     r7, #0
        mov     r8, #0

        ldr     r4, =_bss_start
        ldr     r9, =_bss_end
        b       zcmp
zstart:
        stm     r4!, { r5-r8 }
        cmp     r4, r9
zcmp:   
        blo     zstart
        
        blx     main
sleep:  
        wfe
        b       sleep
