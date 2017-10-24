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
 ; File:   _bitfliplloop.asm
 ; Author: Anthony Campbell
 ;
 ; Created on 05 October 2017, 11:13
 ;

%ifdef NOASSEMBLR
%else

%ifdef WIN64
%idefine rip rel $
%define ARG3 r8
%define ARG2  rdx
%define ARG1  rcx
%else
%define ARG3 rdx
%define ARG2 rsi
%define ARG1 rdi
%endif
bits 64
global _bitflipllloop

section .data

section .text
%ifndef WIN64
    _bitflipllloop:
        mov    eax,esi
        test   rax,rax
        je     _bitflipllloopreturn
        lea    r10,[ARG1 + rax * 8]
        mov    r9d, 3fh
        nop    DWORD  [rax+ 0h ]
        mov    r8,QWORD  [ARG1]
        xor    edx,edx
        xor    esi,esi
        nop
        mov    ecx,edx
        mov    rax,r8
        shr    rax,cl
        mov    ecx,r9d
        sub    ecx,edx
        and    eax,0x1
        add    edx,0x1
        shl    rax,cl
        or     ARG2,rax
        cmp    edx,0x40
        jne    _bitflipllloop + 1ch
        mov    QWORD  [ARG1],ARG2
        add    ARG1,0x8
        cmp    r10,ARG1
        jne    _bitflipllloop + 14h

    _bitflipllloopreturn:
        repz ret
        nop    DWORD  [rax+0x0]
%else
_bitflipllloop:
           push   rbx
           test   edx,edx
           je    _bitflipllloopreturn
           lea    eax, [rdx - 1h]
           lea    r8, [rip]
           lea    r10, [rcx + rax * 4 + 4h]
           mov    edx, DWORD  [rcx]
           add    rcx, 4h
           movzx  eax, dl
           mov    r9d, edx
           movzx  ebx, dh
           shr    edx, 10h
           movzx  eax, BYTE  [r8 + rax * 1]
           shr    r9d, 18h
           movzx  edx, dl
           movzx  r9d, BYTE  [r8 + r9 * 1]
           movzx  edx, BYTE  [r8 + rdx * 1]
           shl    eax,18h
           or     eax, r9d
           movzx  r9d, BYTE  [r8 + rbx * 1]
           shl    edx, 8h
           shl    r9d, 10h
           or     eax, r9d
           or     eax, edx
           mov    DWORD  [rcx - 4h], eax
           cmp    r10, rcx
           jne    _bitflipllloop + 14h
    _bitflipllloopreturn:
           pop    rbx
           ret
           nop    DWORD  [rax]
%endif
%endif
