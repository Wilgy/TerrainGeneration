#
# Definitions
#

CC = gcc
RM = rm -f

#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE = -I/usr/include/SOIL -I./include
LIBDIRS = 

LDLIBS = -lSOIL -lglut -lGL -lm -lGLEW

#
# Compilation and linking flags
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#

OBJDIR = obj
SRCDIR = src

CFLAGS = -g -std=c99 -Wall $(INCLUDE) -DGL_GLEXT_PROTOTYPES

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS)

_C_FILES = cgChunk.c floatVector.c simpleShape.c
C_FILES =	$(patsubst %,$(SRCDIR)/%,$(_C_FILES))

H_FILES = cgChunk.h floatVector.h simpleShape.h

SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)

_OBJFILES = cgChunk.o floatVector.o simpleShape.o
OBJFILES =	$(patsubst %,$(OBJDIR)/%,$(_OBJFILES))

#
# Main targets
#

main:	$(OBJFILES)

#
# Dependencies
#
$(OBJDIR)/%.o: $(SRCDIR)/%.c 
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