;From Naive Long Long Loop;
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
global _bitflipllloop

section .data

section .text
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
%endif
;_bitflipllloop