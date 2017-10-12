cmake_minimum_required(VERSION 3.9)
project(TestApp)

set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)
include(../findBenchmark.cmake)

 


find_package(GTest)
find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)

IF(NOT GTest_FOUND)
message("-- Current build will download GTest")

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

  ELSE(NOT GTest_FOUND)
ENDIF(NOT GTest_FOUND)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage." FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message("-- Current build type is : ${CMAKE_BUILD_TYPE}")

enable_testing()
include(CTest)


include_directories(${GTEST_INCLUDE_DIRS})

set(GCC_BITFLIP_COMPILE_FLAGS "-fopenmp -m64 -march=native")
set(GCC_BITFLIP_LINK_FLAGS "-fopenmp -m64 -march=native")

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_BITFLIP_COMPILE_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_BITFLIP_LINK_FLAGS}" )

file(GLOB INC_FILES ../include/*.h)
file(GLOB SRC_FILES *.cpp)

set(HEADER_FILES ${INC_FILES} ${benchmark_INCLUDE_DIR}/benchmark/benchmark.h)
message("-- Google Benchmark Libraries: ${benchmark_LIBRARIES}")
message("-- Google Benchmark Headers: ${benchmark_INCLUDE_DIRS}")
message("-- Google Test Libraries: ${GTEST_BOTH_LIBRARIES}")
message("-- Google Test Headers: ${GTEST_INCLUDE_DIRS}")

add_library(libgtest IMPORTED STATIC GLOBAL)
add_library(libbenchmark IMPORTED STATIC GLOBAL)

add_executable(TestApp ${SRC_FILES} ${HEADER_FILES})
IF(NOT GTest_FOUND)
add_dependencies(TestApp gtest)
ENDIF(NOT GTest_FOUND)
target_link_libraries(TestApp bitFlip ${GTEST_BOTH_LIBRARIES} ${benchmark_LIBRARIES})

add_test(TestApp TestApp)
