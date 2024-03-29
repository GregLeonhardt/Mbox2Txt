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
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../LibTools/dist/Debug/GNU-Linux/liblibtools.a -lpthread -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbox2txt

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbox2txt: ../LibTools/dist/Debug/GNU-Linux/liblibtools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbox2txt: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbox2txt ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main/main.o: main/main.c
	${MKDIR} -p ${OBJECTDIR}/main
	${RM} "$@.d"
	$(COMPILE.c) -g -I../LibTools/include -Iinclude -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main/main.o main/main.c

# Subprojects
.build-subprojects:
	cd ../LibTools && ${MAKE} -s -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../LibTools && ${MAKE} -s -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
