########################
#  generator makefile  #
########################

# build options
# -Ofast -mfpmath=both -march=native -flto
# -Ofast -mfpmath=both -march=native
# -g -Og
BUILDOPT = -Ofast -mfpmath=both -march=native
# output file
OUTPUT   = ./generator

# code folder
SOURCE = src terrain/structures

# resource file
ifeq ($(OS),Windows_NT)
RESOURCES = res/generator.rc
endif

##############################################################

# compiler
CC = g++ $(BUILDOPT) -std=c++11
# compiler flags
CCFLAGS = -c -Wall -Wextra -pedantic -Iinc
# linker flags
LFLAGS  = -Llib -llibrary -llzo2 -Wl,-rpath,../lib
ifeq ($(OS),Windows_NT)
	LFLAGS  = -Llib -static -llibrary -lpthread -llzo2
endif
# resource builder
RES = windres
# resource builder flags
RFLAGS = -O coff

##############################################################

# make pipeline
DIRECTORIES = $(SOURCE)
CCDIRS  = $(foreach dir, $(DIRECTORIES), $(dir)/*.c)
CCMODS  = $(wildcard $(CCDIRS))
CXXDIRS = $(foreach dir, $(DIRECTORIES), $(dir)/*.cpp)
CXXMODS = $(wildcard $(CXXDIRS))

# compile each .c to .o
.c.o:
	$(CC) $(CCFLAGS) $< -o $@

# compile each .cpp to .o
.cpp.o:
	$(CC) $(CCFLAGS) $< -o $@

# recipe for building .o from .rc files
%.o : %.rc
	$(RES) $(RFLAGS) $< -o $@

# convert .c to .o
CCOBJS  = $(CCMODS:.c=.o)
# convert .cpp to .o
CXXOBJS = $(CXXMODS:.cpp=.o)
# resource .rc to .o
CCRES   = $(RESOURCES:.rc=.o)

.PHONY: clean all

# link all OBJS using CC and link with LFLAGS, then output to OUTPUT
all: $(CXXOBJS) $(CCOBJS) $(CCRES)
	$(CC) $(CXXOBJS) $(CCOBJS) $(CCRES) $(LFLAGS) -o $(OUTPUT)

# remove each known .o file, and output
clean:
	$(RM) $(CXXOBJS) $(CCOBJS) $(CCRES) *~ $(OUTPUT).*
