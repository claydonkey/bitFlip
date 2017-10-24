 ;
 ; The MIT License
 ;
 ; Copyright 2017 Anthony Campbell.
 ;
 ; Permission is hereby granted, free of charge, to any person obtaining a copy
 ; of this software and associated documentation files (the "Software"), to deal
 ; in the Software without restriction, including without limitation the rights
 ; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ; copies of the Software, and to permit persons to whom the Software is
 ; furnished to do so, subject to the following conditions:
 ;
 ; The above copyright notice and this permission notice shall be included in
 ; all copies or substantial portions of the Software.
 ;
 ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 ; THE SOFTWARE.
 ;
 ;
 ; @author Anthony Campbell contact@claydonkey.com
 ;
 ;
 ; File:   _bitflipbyte.asm
 ; Author: Anthony Campbell
 ;
 ; Created on 05 October 2017, 11:13
 ;

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
