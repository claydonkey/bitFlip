;_bitflipbyte
%ifdef NOASSEMBLR
%else

%ifdef WIN64
%define ARG3 r8
%define ARG2  rdx
%define ARG1  rcx
%else
%define ARG3 rdx
%define ARG2 rsi
%define ARG1 rdi
%endif

    bits 64
    global _bitflipbyte

    section .data

    section .text

    _bitflipbyte:
            vmovdqa     ymm2, [ARG3]  ;k1
            add         ARG3, 20h
            vmovdqa     ymm3, [ARG3]   ;k2
            add         ARG3, 20h
            vmovdqa     ymm4, [ARG3]   ;k3
    _bitflipp_loop:
            vmovdqa     ymm0, [ARG1]
            vpand       ymm1, ymm2, ymm0   ; ymm1 = arg1 and k1
            vpandn      ymm0, ymm2, ymm0  ; arg1 = arg1 and not k1
            vpsrld      ymm0, ymm0, 4h  ;   ;shift arg1 4
            vpshufb     ymm1, ymm4, ymm1  ;ymm1 =  shuffle  k3 ymm1
            vpshufb     ymm0, ymm3, ymm0  ;arg1 = shuffle    k2 arg1
            vpor        ymm0, ymm0, ymm1
            vmovdqa     [ARG1], ymm0
            add     ARG1, 20h
            dec     ARG2
            jnz     _bitflipp_loop
        ret
%endif
