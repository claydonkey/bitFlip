;From Naive Long Long Loop;
bits 64
global bitflipllloop

section .data

section .text

bitflipllloop:
    mov    eax,esi
    test   rax,rax
    je     bitflipllloopreturn
    lea    r10,[rdi + rax * 8]
    mov    r9d, 3fh
    nop    DWORD  [rax+ 0h ]
    mov    r8,QWORD  [rdi]
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
    or     rsi,rax
    cmp    edx,0x40
    jne    bitflipllloop + 1ch
    mov    QWORD  [rdi],rsi
    add    rdi,0x8
    cmp    r10,rdi
    jne    bitflipllloop + 14h

bitflipllloopreturn:
    repz ret 
    nop    DWORD  [rax+0x0]


