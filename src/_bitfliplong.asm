bits 64
global _bitfliplong

section .data

section .text

_bitfliplong:
xor    esi,esi
xor    eax,eax
mov    r8d,0x3f
nop    WORD   [rax + rax * 1 + 0h]
_bitfliplongLoop:
mov    ecx,esi
mov    rdx,rdi
shr    rdx,cl
mov    ecx,r8d
sub    ecx,esi
and    edx,0x1
add    esi,0x1
shl    rdx,cl
or     rax,rdx
cmp    esi,0x40
jne    _bitfliplongLoop + 10h
repz ret