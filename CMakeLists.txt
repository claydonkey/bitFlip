cmake_minimum_required(VERSION 3.9)
project(bitFlip)

set(CMAKE_CXX_STANDARD 11)

include_directories(./include)
add_subdirectory(tests)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message("-- Current build type is : ${CMAKE_BUILD_TYPE}")

set(SOURCE_FILES
        include/bitFlip.h
        include/BitReverseTable16.h
        include/BitReverseTable8.h
        src/bitFlip.cpp
        src/bitflipbyte.asm
        src/bitfliplong.asm
        )

find_program(NASM_EXE NAMES nasm)

add_custom_command(OUTPUT bitflipbyte.o COMMAND ${NASM_EXE} ARGS -f elf64 -o bitflipbyte.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipbyte.asm)
add_custom_command(OUTPUT bitfliplong.o COMMAND ${NASM_EXE} ARGS -f elf64 -o bitfliplong.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitfliplong.asm)
add_custom_command(OUTPUT bitflipllloop.o COMMAND ${NASM_EXE} ARGS -f elf64 -o bitflipllloop.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipllloop.asm)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(HEADER_FILES ./include/BitReverseTable8.h ./include/BitReverseTable16.h  ./include/bitFlip.h )

add_library(bitFlip SHARED bitfliplong.o bitflipbyte.o bitflipllloop.o ${SOURCE_FILES} ${HEADER_FILES})
