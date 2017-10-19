cmake_minimum_required(VERSION 3.9)
project(bitFlip C CXX)

include(CMakePackageConfigHelpers)
set(CMAKE_CXX_STANDARD 11)



IF(MINGW)
	set (ASM_FORMAT win64)
	set(USE_ASM false)
ELSE(MINGW)
	set (ASM_FORMAT elf64)
	set (USE_ASM true)
ENDIF(MINGW)

set(TESTAPP_USE_ASM ${USE_ASM} CACHE BOOL 1)
set_property(CACHE TESTAPP_USE_ASM PROPERTY STRINGS 1 0)

IF(NOT TESTAPP_USE_ASM)
	set(ASM_FLAG "-DNOASSMBLR")
        message("Current build will not build assembler files.")
ELSE(NOT TESTAPP_USE_ASM)
        message(STATUS "Current build includes assembler files.")
ENDIF(NOT TESTAPP_USE_ASM)

set( TESTAPP_ASMFORMAT ${ASM_FORMAT} CACHE STRING "Assembly Format")
set_property(CACHE TESTAPP_ASMFORMAT PROPERTY STRINGS elf64 win64)

include_directories(./include)
add_subdirectory(tests)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(SOURCE_FILES
	include/bitFlip.h
	include/BitReverseTable16.h
	include/BitReverseTable8.h
	src/bitFlip.cpp
	)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native  ${ASM_FLAG}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native  ${ASM_FLAG}")


IF(TESTAPP_USE_ASM)
	set(BITFLIP_ASM_FILES
		src/_bitflipbyte.asm
		)
#IF(UNIX)
#set(BITFLIP_ASM_FILES ${BITFLIP_ASM_FILES}
#		src/_bitflipllloop.asm
#)
#ENDIF(UNIX)

	find_program(NASM_EXE NAMES nasm REQUIRED)

	enable_language(ASM_NASM)

IF(UNIX)
    SET(CMAKE_ASM_NASM_FLAGS ${CMAKE_ASM_NASM_FLAGS}  "")
ELSE(UNIX)
    SET(CMAKE_ASM_NASM_FLAGS  ${CMAKE_ASM_NASM_FLAGS} "-dWIN64")
ENDIF(UNIX)
	#set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS}  -f  ${TESTAPP_ASMFORMAT}")
	IF(CMAKE_ASM_NASM_COMPILER_LOADED)

		set(CAN_USE_ASSEMBLER TRUE)
		set(BITFLIP_SOURCES  ${BITFLIP_ASM_FILES})

	ENDIF(CMAKE_ASM_NASM_COMPILER_LOADED)

ENDIF(TESTAPP_USE_ASM)

message(STATUS "Google Test build type is : ${CMAKE_BUILD_TYPE}")
message(STATUS "Assembler Format is ${TESTAPP_ASMFORMAT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(HEADER_FILES ./include/BitReverseTable8.h ./include/BitReverseTable16.h  ./include/bitFlip.h )

#CREATE LIBRARY
add_library(bitFlip SHARED ${BITFLIP_ASM_FILES} ${SOURCE_FILES} ${HEADER_FILES})

#INSTALL CMAKE MODULES AND LIBRARY
set(LIB_DESTINATION "${CMAKE_INSTALL_LIBDIR}")
set(CMAKECONFIG_INSTALL_DIR "${LIB_DESTINATION}/cmake/bitflip")
set(INCLUDE_INSTALL_DIR "include")

## LIBRARY VERSION
set(bitFlip_SOVERSION 1)
set(bitFlip_API_VERSION 0)
set(bitFlip_lib_PATCH_VERSION 0)

set(bitFlip_VERSION ${bitFlip_SOVERSION}.${bitFlip_API_VERSION}.${bitFlip_PATCH_VERSION})

# GENEREATE btstack-config* FILES
configure_package_config_file(
	bitFlip-config.cmake.in
	${CMAKE_BINARY_DIR}/bitFlip-config.cmake
	INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR
	)

write_basic_package_version_file(
	${CMAKE_BINARY_DIR}/bitFlip-config-version.cmake
	VERSION ${bitFlip_VERSION}
	COMPATIBILITY SameMajorVersion
	)

install(EXPORT bitFlip-targets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE bitFlip-targets.cmake )

file(GLOB files "include/*.h")
install(FILES ${files} DESTINATION include)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
set(LIB_DESTINATION "${CMAKE_INSTALL_LIBDIR}")

set(CMAKECONFIG_INSTALL_DIR "${LIB_DESTINATION}/cmake")
set(INCLUDE_INSTALL_DIR "include")


install(FILES
	${CMAKE_BINARY_DIR}/bitFlip-config.cmake
	${CMAKE_BINARY_DIR}/bitFlip-config-version.cmake
	DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
	COMPONENT Devel
	)

install(TARGETS bitFlip
	EXPORT bitFlip-targets
	ARCHIVE DESTINATION ${LIB_DESTINATION}
	LIBRARY DESTINATION ${LIB_DESTINATION}
	RUNTIME DESTINATION bin
	)
