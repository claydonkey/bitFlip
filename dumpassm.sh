#!/bin/sh 
objdump -d -M intel --no-show-raw-insn  build/MinGW64_Release/x86_64-w64-mingw32-Windows/src/bitFlip.o >> assm.txt
