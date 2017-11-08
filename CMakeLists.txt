cmake_minimum_required(VERSION 3.9)
project(bitFlip C CXX)

include(CMakePackageConfigHelpers)
set(CMAKE_CXX_STANDARD 11)

IF(MINGW)
	set (ASM_FORMAT win64)
	set(USE_ASM true)
ELSE(MINGW)
	set (ASM_FORMAT elf64)
	set (USE_ASM true)
ENDIF(MINGW)

set (USE_TABLEHEADERS  true)
set (USE_BENCHMARK  true)

set(TESTAPP_USE_ASM ${USE_ASM} CACHE BOOL 1)
set_property(CACHE TESTAPP_USE_ASM PROPERTY STRINGS 1 0)

set(TESTAPP_USE_TABLEHEADERS ${USE_TABLEHEADERS} CACHE BOOL 1)
set_property(CACHE TESTAPP_USE_TABLEHEADERS PROPERTY STRINGS 1 0)

set(TESTAPP_USE_BENCHMARK ${USE_BENCHMARK} CACHE BOOL 1)
set_property(CACHE TESTAPP_USE_BENCHMARK PROPERTY STRINGS 1 0)

IF( TESTAPP_USE_TABLEHEADERS)
	set(TABLE_FLAG "-DTABLEHEADERS")
        message(STATUS "Current build will use table headers.")
ELSE(TESTAPP_USE_TABLEHEADERS)
        message(STATUS "Current build will not use table headers.")
ENDIF(TESTAPP_USE_TABLEHEADERS)

IF( TESTAPP_USE_BENCHMARK)
	        message(STATUS "Current build will run benchmarks.")
ELSE(TESTAPP_USE_BENCHMARK)
set(BENCHMARK_FLAG "-DNOBENCHMARK")
        message(STATUS "Current build will not run benchmarks.")
ENDIF(TESTAPP_USE_BENCHMARK)

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

execute_process(COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/get_version.sh")
set_directory_properties(PROPERTIES  ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_SOURCE_DIR}/include/bitFlipVersion.h")

set(HEADER_FILES
	include/bitFlip.h
	include/bitFlipTables.h
	include/bitFlipAVX.h
	include/typeDeduce.h
	include/bitFlipVersion.h
 )

set(SOURCE_FILES
	${HEADER_FILES}
	src/bitFlip.cpp
	src/bitFlipAVX.cpp
)

set(GCC_BITFLIP_COMPILE_C_FLAGS "${CMAKE_C_FLAGS}  -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native ${BENCHMARK_FLAG} ${TABLE_FLAG}  ${ASM_FLAG}")
set(GCC_BITFLIP_COMPILE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native  ${BENCHMARK_FLAG} ${TABLE_FLAG}  ${ASM_FLAG}")
set(GCC_BITFLIP_LINK_FLAGS  "${GCC_BITFLIP_LINK_FLAGS} -O3 -pipe -fomit-frame-pointer -mavx -m64 -fopenmp -m64 -march=native ${BENCHMARK_FLAG} ${TABLE_FLAG}  ${ASM_FLAG}")


IF(MINGW)
	set(GCC_BITFLIP_COMPILE_CXX_FLAGS "${GCC_BITFLIP_COMPILE_CXX_FLAGS} -static-libstdc++ ")#   -Wl,--subsystem,windows")
	set(GCC_BITFLIP_COMPILE_C_FLAGS "${GCC_BITFLIP_COMPILE_C_FLAGS} -static-libgcc  ")#  -Wl,--subsystem,windows")
	set(GCC_BITFLIP_LINK_FLAGS "${GCC_BITFLIP_LINK_FLAGS}   -Wl,--subsystem,windows")
ENDIF(MINGW)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_BITFLIP_COMPILE_C_FLAGS}" )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_BITFLIP_COMPILE_CXX_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_BITFLIP_LINK_FLAGS}" )


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