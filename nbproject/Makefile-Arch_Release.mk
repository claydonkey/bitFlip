#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=nasm

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Arch_Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/bitFlip.o \
	${OBJECTDIR}/bitflipbyte.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/bitFliptSuite.o \
	${TESTDIR}/tests/bitFlipTest.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-f elf64

# Link Libraries and Options
LDLIBSOPTIONS=-lgtest

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fopenmp -m64 -march=native -shared -fPIC

${OBJECTDIR}/bitFlip.o: bitFlip.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -I/C/msys2/mingw64/include -I/usr/include -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bitFlip.o bitFlip.cpp

${OBJECTDIR}/bitflipbyte.o: bitflipbyte.asm
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/bitflipbyte.o bitflipbyte.asm

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/bitFlipTest.o ${TESTDIR}/bitFliptSuite.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   -lgtest -lbenchmark -lpthread 


${TESTDIR}/tests/bitFlipTest.o: tests/bitFlipTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -I/C/msys2/mingw64/include -I/usr/include -I/usr/include -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitFlipTest.o tests/bitFlipTest.cpp


${TESTDIR}/bitFliptSuite.o: bitFliptSuite.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -I/C/msys2/mingw64/include -I/usr/include -I/usr/include -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/bitFliptSuite.o bitFliptSuite.cpp


${OBJECTDIR}/bitFlip_nomain.o: ${OBJECTDIR}/bitFlip.o bitFlip.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bitFlip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -s -I/C/msys2/mingw64/include -I/usr/include -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bitFlip_nomain.o bitFlip.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bitFlip.o ${OBJECTDIR}/bitFlip_nomain.o;\
	fi

${OBJECTDIR}/bitflipbyte_nomain.o: ${OBJECTDIR}/bitflipbyte.o bitflipbyte.asm 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bitflipbyte.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    $(AS) $(ASFLAGS) -Dmain=__nomain -o ${OBJECTDIR}/bitflipbyte_nomain.o bitflipbyte.asm;\
	else  \
	    ${CP} ${OBJECTDIR}/bitflipbyte.o ${OBJECTDIR}/bitflipbyte_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
