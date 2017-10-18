cmake_minimum_required(VERSION 3.9)
project(TestApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(ExternalProject)
find_package(GTest)
find_package(Threads REQUIRED)
find_package(benchmark)

IF(MINGW)
	find_package(shlwapi REQUIRED )
ENDIF( MINGW)

IF(NOT GTest_FOUND)
	message(STATUS "Current build will download GTest")

	ExternalProject_Add(
		gtest
		URL https://github.com/google/googletest/archive/master.zip
		CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=Release
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
		INSTALL_COMMAND "")

	set(GTEST_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest/src/gtest-build/googlemock/gtest)
	ExternalProject_Get_Property(gtest source_dir binary_dir)

	set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/gtest/src/gtest/googletest/include)
	set(GTEST_BOTH_LIBRARIES ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a)
ELSE()
    #message(STATUS "Found: gtest")
ENDIF(NOT GTest_FOUND)

IF(NOT benchmark_FOUND)
	message(STATUS "Current build will download google benchmark")

	ExternalProject_Add(
		benchmark
		URL https://github.com/google/benchmark/archive/master.zip
		CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=Release
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}/benchmark
		INSTALL_COMMAND "")

	set(BENCHMARK_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/benchmark/src/benchmark-build/src)
	ExternalProject_Get_Property(benchmark source_dir binary_dir)

	set(benchmark_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/benchmark/src/benchmark/include)
	set(benchmark_LIBRARIES ${BENCHMARK_LIB_DIR}/libbenchmark.a )

ENDIF(NOT benchmark_FOUND)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

message(STATUS "Library Build type is : ${CMAKE_BUILD_TYPE}")

#enable_testing()
#include(CTest)

include_directories(${benchmark_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

set(GCC_BITFLIP_COMPILE_FLAGS "-O3 -pipe -fomit-frame-pointer -fopenmp -mavx -m64 -march=native ${ASM_FLAG}")
set(GCC_BITFLIP_LINK_FLAGS "-O3 -pipe -fomit-frame-pointer -fopenmp -mavx -m64 -march=native ${ASM_FLAG}")

IF(MINGW)
	set(GCC_BITFLIP_COMPILE_FLAGS "${GCC_BITFLIP_COMPILE_FLAGS}  -Wl,--subsystem,windows")
	set(GCC_BITFLIP_LINK_FLAGS "${GCC_BITFLIP_LINK_FLAGS}  -Wl,--subsystem,windows")
ENDIF(MINGW)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_BITFLIP_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_BITFLIP_LINK_FLAGS}" )

file(GLOB INC_FILES ../include/*.h)
file(GLOB SRC_FILES *.cpp)

#set(HEADER_FILES ${INC_FILES} ${benchmark_INCLUDE_DIR}/benchmark/benchmark.h)
#message(STATUS "Google Benchmark Libraries: ${benchmark_LIBRARIES}")
#message(STATUS "Google Benchmark Headers: ${benchmark_INCLUDE_DIRS}")
#message(STATUS "Google Test Libraries: ${GTEST_BOTH_LIBRARIES}")
#message(STATUS "Google Test Headers: ${GTEST_INCLUDE_DIRS}")

#add_library(libgtest IMPORTED STATIC GLOBAL)
#add_library(libbenchmark IMPORTED STATIC GLOBAL)
#add_library(libbitFlip IMPORTED SHARED)


add_executable(TestApp ${SRC_FILES} ${HEADER_FILES})
IF(MINGW)
add_custom_command(TARGET TestApp POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different     "../libbitFlip.dll"        $<TARGET_FILE_DIR:TestApp>
)
ENDIF(MINGW)
IF(NOT benchmark_FOUND)
	add_dependencies(TestApp benchmark)
ENDIF(NOT  benchmark_FOUND)
IF(NOT GTest_FOUND)
	add_dependencies(TestApp gtest)
ENDIF(NOT GTest_FOUND)
target_link_libraries(TestApp bitFlip  ${GTEST_BOTH_LIBRARIES} ${benchmark_LIBRARIES} ${shlwapi_LIBRARIES})

add_test(TestApp TestApp)
