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
CC=x86_64-w64-mingw32-gcc
CCC=x86_64-w64-mingw32-g++
CXX=x86_64-w64-mingw32-g++
FC=x86_64-w64-mingw32-gfortran
AS=nasm

# Macros
CND_PLATFORM=x86_64-w64-mingw32-Windows
CND_DLIB_EXT=dll
CND_CONF=MinGW64_Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/_bitflipbyte.o \
	${OBJECTDIR}/src/bitFlip.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/bitFlipTest.o \
	${TESTDIR}/tests/bitFlipTestSuite.o

# C Compiler Flags
CFLAGS=-mavx2 -m64 -fopenmp -m64 -march=native

# CC Compiler Flags
CCFLAGS=-mavx2 -m64 -fopenmp -m64 -march=native
CXXFLAGS=-mavx2 -m64 -fopenmp -m64 -march=native

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-f win64 -dYESASSEMBLR -dWIN64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -Wl,--subsystem,windows,--output-def,${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.def,--out-implib,${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.dll.a -shared

${OBJECTDIR}/src/_bitflipbyte.o: src/_bitflipbyte.asm
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/_bitflipbyte.o src/_bitflipbyte.asm

${OBJECTDIR}/src/bitFlip.o: src/bitFlip.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DYESASSMBLR -Iinclude  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlip.o src/bitFlip.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/bitFlipTest.o ${TESTDIR}/tests/bitFlipTestSuite.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}  -fopenmp -mavx -m64 -march=native -Wl,--subsystem,windows -Wl,--subsystem,windows -lpthread -lbenchmark -lgtest -lshlwapi -Wl,--subsystem,windows 


${TESTDIR}/tests/bitFlipTest.o: tests/bitFlipTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DYESASSMBLR -DNOBENCHMARK -Iinclude -I../include -fopenmp -mavx -m64 -march=native -Wl,--subsystem,windows -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitFlipTest.o tests/bitFlipTest.cpp


${TESTDIR}/tests/bitFlipTestSuite.o: tests/bitFlipTestSuite.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DYESASSMBLR -DNOBENCHMARK -Iinclude -I../include -fopenmp -mavx -m64 -march=native -Wl,--subsystem,windows -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitFlipTestSuite.o tests/bitFlipTestSuite.cpp


${OBJECTDIR}/src/_bitflipbyte_nomain.o: ${OBJECTDIR}/src/_bitflipbyte.o src/_bitflipbyte.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/_bitflipbyte.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    $(AS) $(ASFLAGS) -Dmain=__nomain -o ${OBJECTDIR}/src/_bitflipbyte_nomain.o src/_bitflipbyte.asm;\
	else  \
	    ${CP} ${OBJECTDIR}/src/_bitflipbyte.o ${OBJECTDIR}/src/_bitflipbyte_nomain.o;\
	fi

${OBJECTDIR}/src/bitFlip_nomain.o: ${OBJECTDIR}/src/bitFlip.o src/bitFlip.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/bitFlip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DYESASSMBLR -Iinclude  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlip_nomain.o src/bitFlip.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/bitFlip.o ${OBJECTDIR}/src/bitFlip_nomain.o;\
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
