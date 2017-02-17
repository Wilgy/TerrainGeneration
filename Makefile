#
# Created by gmakemake (Ubuntu Oct  3 2011) on Fri Feb 17 14:34:34 2017
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.s) -o $@ $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

AS =		as
CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.s = $(AS) $(ASFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLUT, GLEW, and/or
# SOIL on the Ubuntu systems.
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE = -I/usr/include/SOIL
LIBDIRS = 

LDLIBS = -lSOIL -lglut -lGL -lm -lGLEW

#
# Compilation and linking flags
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
CFLAGS = -g -std=c99 -Wall $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CCFLAGS =  $(CFLAGS)
CXXFLAGS = $(CFLAGS)

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS)
CCLIBFLAGS = $(LIBFLAGS)

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	blockInfo.c cgCube.c cgSquare.c finalMain.c floatVector.c lightingParams.c shaderSetup.c simpleShape.c textureParams.c viewParams.c
PS_FILES =	
S_FILES =	
H_FILES =	blockInfo.h cgCube.h cgSquare.h floatVector.h lightingParams.h shaderSetup.h simpleShape.h textureParams.h viewParams.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	blockInfo.o cgCube.o cgSquare.o floatVector.o lightingParams.o shaderSetup.o simpleShape.o textureParams.o viewParams.o 

#
# Main targets
#

all:	finalMain 

finalMain:	finalMain.o $(OBJFILES)
	$(CC) $(CFLAGS) -o finalMain finalMain.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

blockInfo.o:	blockInfo.h
cgCube.o:	simpleShape.h
cgSquare.o:	simpleShape.h
finalMain.o:	blockInfo.h cgSquare.h lightingParams.h shaderSetup.h simpleShape.h textureParams.h viewParams.h
floatVector.o:	floatVector.h
lightingParams.o:	lightingParams.h
shaderSetup.o:	shaderSetup.h
simpleShape.o:	floatVector.h simpleShape.h
textureParams.o:	textureParams.h
viewParams.o:	viewParams.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) finalMain.o core

realclean:        clean
	-/bin/rm -f finalMain 
