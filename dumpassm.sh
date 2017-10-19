#!/bin/sh 

if [ $1 == win ]; then
objdump -d -M intel --no-show-raw-insn  build/MinGW64_Release/x86_64-w64-mingw32-Windows/src/bitFlip.o > assm.txt
elif [ $1 == linux ]; then
objdump -d -M intel --no-show-raw-insn build/Arch_Release/GNU-Linux/src/bitFlip.o > assm.txt
fi
