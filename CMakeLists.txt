cmake_minimum_required(VERSION 3.9)
project(bitFlip)

include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 11)


set_property(CACHE TESTAPP_ASMFORMAT PROPERTY STRINGS elf64 win64)
message(STATUS "Assm Format='${TESTAPP_ASMFORMAT}'")

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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}-O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native")

IF(MINGW)

	set(TESTAPP_ASMFORMAT "win64" CACHE STRING "Assembly Architecture to compile to")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--subsystem,windows,--output-def,libbitFlip.def,--out-implib,libbitFlip.dll.a")

ELSE(MINGW)

	set(TESTAPP_ASMFORMAT "elf64" CACHE STRING "Assembly Architecture to compile to")
	set(SOURCE_FILES
		${SOURCE_FILES}
		src/bitflipbyte.asm
		src/bitfliplong.asm)
	find_program(NASM_EXE NAMES nasm)
	add_custom_command(OUTPUT bitflipbyte.o COMMAND ${NASM_EXE} ARGS -f ${TESTAPP_ASMFORMAT} -o bitflipbyte.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipbyte.asm)
	add_custom_command(OUTPUT bitfliplong.o COMMAND ${NASM_EXE} ARGS -f  ${TESTAPP_ASMFORMAT} -o bitfliplong.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitfliplong.asm)
	add_custom_command(OUTPUT bitflipllloop.o COMMAND ${NASM_EXE} ARGS -f  ${TESTAPP_ASMFORMAT} -o bitflipllloop.o ${CMAKE_CURRENT_SOURCE_DIR}/src/bitflipllloop.asm)
	set(bitflip_ASSEMBLIES  bitfliplong.o bitflipbyte.o bitflipllloop.o)

ENDIF(MINGW)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(HEADER_FILES ./include/BitReverseTable8.h ./include/BitReverseTable16.h  ./include/bitFlip.h )

#for installation and cmake findpackage

set(LIB_DESTINATION "${CMAKE_INSTALL_LIBDIR}")
set(CMAKECONFIG_INSTALL_DIR "${LIB_DESTINATION}/cmake/bitflip")
set(INCLUDE_INSTALL_DIR "include")

add_library(bitFlip SHARED ${bitflip_ASSEMBLIES} ${SOURCE_FILES} ${HEADER_FILES})

# Generate btstack-config* files
configure_package_config_file(
	bitFlip-config.cmake.in
	${CMAKE_BINARY_DIR}/bitFlip-config.cmake
	INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR
	)

## Lib version
set(bitFlip_lib_SOVERSION 1)
set(bitFlip_lib_API_VERSION 0)
set(bitFlip_lib_PATCH_VERSION 0)

set(bitFlip_lib_VERSION ${bitFlip_lib_SOVERSION}.${bitFlip_lib_API_VERSION}.${bitFlip_lib_PATCH_VERSION})

write_basic_package_version_file(
	${CMAKE_BINARY_DIR}/bitFlip-config-version.cmake
	VERSION ${bitFlip_lib_VERSION}
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



