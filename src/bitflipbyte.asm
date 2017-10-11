bits 64
global bitflipbyte

section .data

section .text

bitflipbyte:
        vmovdqa     ymm2, [rdx]
        add         rdx, 20h
        vmovdqa     ymm3, [rdx]
        add         rdx, 20h
        vmovdqa     ymm4, [rdx]

bitflipp_loop:
        vmovdqa     ymm0, [rdi]
        vpand       ymm1, ymm2, ymm0
        vpandn      ymm0, ymm2, ymm0
        vpsrld      ymm0, ymm0, 4h
        vpshufb     ymm1, ymm4, ymm1
        vpshufb     ymm0, ymm3, ymm0
        vpor        ymm0, ymm0, ymm1
        vmovdqa     [rdi], ymm0
        add     rdi, 20h
        dec     rsi
        jnz     bitflipp_loop
        ret