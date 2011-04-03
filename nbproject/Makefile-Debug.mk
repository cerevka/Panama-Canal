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
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/1128534263/Enviroment.o \
	${OBJECTDIR}/_ext/1128534263/Camera.o \
	${OBJECTDIR}/_ext/1128534263/Light.o


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
LDLIBSOPTIONS=-lglut -lGLU

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/panamacanal

dist/Debug/GNU-Linux-x86/panamacanal: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/panamacanal ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/1128534263/Enviroment.o: /media/data/Documents/CTU/6/pgr/PanamaCanal/Enviroment.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1128534263
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1128534263/Enviroment.o /media/data/Documents/CTU/6/pgr/PanamaCanal/Enviroment.cpp

${OBJECTDIR}/_ext/1128534263/Camera.o: /media/data/Documents/CTU/6/pgr/PanamaCanal/Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1128534263
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1128534263/Camera.o /media/data/Documents/CTU/6/pgr/PanamaCanal/Camera.cpp

${OBJECTDIR}/_ext/1128534263/Light.o: /media/data/Documents/CTU/6/pgr/PanamaCanal/Light.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1128534263
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1128534263/Light.o /media/data/Documents/CTU/6/pgr/PanamaCanal/Light.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/panamacanal

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
