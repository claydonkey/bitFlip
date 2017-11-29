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
CND_CONF=Arch_Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/_bitflipbyte.o \
	${OBJECTDIR}/src/_bitflipllloop.o \
	${OBJECTDIR}/src/bitFlip.o \
	${OBJECTDIR}/src/bitFlipAVX.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/bitFlipTest.o \
	${TESTDIR}/tests/bitFlipTestSuite.o \
	${TESTDIR}/tests/flipBits.o

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

${OBJECTDIR}/src/_bitflipbyte.o: src/_bitflipbyte.asm
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/_bitflipbyte.o src/_bitflipbyte.asm

${OBJECTDIR}/src/_bitflipllloop.o: src/_bitflipllloop.asm
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/_bitflipllloop.o src/_bitflipllloop.asm

${OBJECTDIR}/src/bitFlip.o: src/bitFlip.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlip.o src/bitFlip.cpp

${OBJECTDIR}/src/bitFlipAVX.o: src/bitFlipAVX.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlipAVX.o src/bitFlipAVX.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/flipBits.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT}: ${TESTDIR}/tests/bitFlipTest.o ${TESTDIR}/tests/bitFlipTestSuite.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT} $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/flipBits.o: tests/flipBits.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/flipBits.o tests/flipBits.cpp


${TESTDIR}/tests/bitFlipTest.o: tests/bitFlipTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -I/usr/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitFlipTest.o tests/bitFlipTest.cpp


${TESTDIR}/tests/bitFlipTestSuite.o: tests/bitFlipTestSuite.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -I/usr/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/bitFlipTestSuite.o tests/bitFlipTestSuite.cpp


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

${OBJECTDIR}/src/_bitflipllloop_nomain.o: ${OBJECTDIR}/src/_bitflipllloop.o src/_bitflipllloop.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/_bitflipllloop.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    $(AS) $(ASFLAGS) -Dmain=__nomain -o ${OBJECTDIR}/src/_bitflipllloop_nomain.o src/_bitflipllloop.asm;\
	else  \
	    ${CP} ${OBJECTDIR}/src/_bitflipllloop.o ${OBJECTDIR}/src/_bitflipllloop_nomain.o;\
	fi

${OBJECTDIR}/src/bitFlip_nomain.o: ${OBJECTDIR}/src/bitFlip.o src/bitFlip.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/bitFlip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlip_nomain.o src/bitFlip.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/bitFlip.o ${OBJECTDIR}/src/bitFlip_nomain.o;\
	fi

${OBJECTDIR}/src/bitFlipAVX_nomain.o: ${OBJECTDIR}/src/bitFlipAVX.o src/bitFlipAVX.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/bitFlipAVX.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DEXTERNALTABLE -Iinclude -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitFlipAVX_nomain.o src/bitFlipAVX.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/bitFlipAVX.o ${OBJECTDIR}/src/bitFlipAVX_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbitFlip.${CND_DLIB_EXT} || true; \
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
