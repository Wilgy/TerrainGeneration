#
# Definitions
#

CC = gcc
RM = rm -f

########## Flags from header.mak

#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLUT, GLEW, and/or
# SOIL on the Ubuntu systems.
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE = -I/usr/include/SOIL -I./datatype/include \
	-I./object/include   \
	-I./shader/include 
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
PS_FILES =	
S_FILES =	
H_FILES =
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)

_DATATYPESRCFILES = $(wildcard datatype/src/*.c)
_DATATYPEOBJFILES = $(patsubst %.c, datatype/obj/%.o, $(notdir $(_DATATYPESRCFILES)))

_OBJECTSRCFILES = $(wildcard object/src/*.c)
_OBJECTOBJFILES = $(patsubst %.c, object/obj/%.o, $(notdir $(_OBJECTSRCFILES)))

_SHADERSRCFILES = $(wildcard shader/src/*.c)
_SHADEROBJFILES = $(patsubst %.c, shader/obj/%.o, $(notdir $(_SHADERSRCFILES)))

C_FILES = main.c $(_DATATYPESRCFILES) $(_OBJECTSRCFILES) $(_SHADERSRCFILES)
OBJFILES =	$(_DATATYPEOBJFILES) $(_OBJECTOBJFILES) $(_SHADEROBJFILES)

#
# Main targets
#
main: $(OBJFILES) $(C_FILES)
	+$(MAKE) -C datatype
	+$(MAKE) -C object
	+$(MAKE) -C shader
	$(CC) $(CFLAGS) -o main main.c $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#
%.o: %.c 
	$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS)

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES)

realclean:        clean
	-/bin/rm -f main 
