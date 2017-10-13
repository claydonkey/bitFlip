cmake_minimum_required(VERSION 3.9)
project(bitFlip)

set(CMAKE_CXX_STANDARD 11)
IF(MINGW)
	set(TESTAPP_ASMFORMAT "win64" CACHE STRING "Assembly Architecture to compile to")
ELSE(MINGW)
	set(TESTAPP_ASMFORMAT "elf64" CACHE STRING "Assembly Architecture to compile to")
ENDIF(MINGW)

set_property(CACHE TESTAPP_ASMFORMAT PROPERTY STRINGS elf64 win64)
message(STATUS "Assm Format='${TESTAPP_ASMFORMAT}'")

find_library(SHLWAPI Shlswapi.lib REQUIRED) 

include_directories(./include)
add_subdirectory(tests)


IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message("-- Current gtest build type is : ${CMAKE_BUILD_TYPE}")

set(SOURCE_FILES
	include/bitFlip.h
	include/BitReverseTable16.h
	include/BitReverseTable8.h
	src/bitFlip.cpp
	)

IF(NOT MINGW)
	set(SOURCE_FILES 
		${SOURCE_FILES}  
		src/bitflipbyte.asm
		src/bitfliplong.asm)
ENDIF(NOT MINGW)


find_program(NASM_EXE NAMES nasm)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}-O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}-O3 -mavx -pipe -fomit-frame-pointer -fopenmp -m64 -march=native -Wl,--subsystem,windows,--output-def,${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.def,--out-implib,${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.dll.a")

IF(NOT MINGW)
	add_custom_command(OUTPUT bitflipbyte.o COMMAND ${NASM_EXE} ARGS -f ${TESTAPP_ASMFORMAT} -o bitflipbyte.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipbyte.asm)
	add_custom_command(OUTPUT bitfliplong.o COMMAND ${NASM_EXE} ARGS -f  ${TESTAPP_ASMFORMAT} -o bitfliplong.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitfliplong.asm)
	add_custom_command(OUTPUT bitflipllloop.o COMMAND ${NASM_EXE} ARGS -f  ${TESTAPP_ASMFORMAT} -o bitflipllloop.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipllloop.asm)
ENDIF(NOT MINGW)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(HEADER_FILES ./include/BitReverseTable8.h ./include/BitReverseTable16.h  ./include/bitFlip.h )

IF(NOT MINGW)
add_library(bitFlip SHARED bitfliplong.o bitflipbyte.o bitflipllloop.o ${SOURCE_FILES} ${HEADER_FILES})
ELSE(NOT MINGW)
add_library(bitFlip SHARED ${SOURCE_FILES} ${HEADER_FILES})
ENDIF(NOT MINGW)