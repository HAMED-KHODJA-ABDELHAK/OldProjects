# Make file for COMP428 assignment 2.
# Just use "make" to build all programs, see respective c files for execution.
# Use "make clean" to remove all built exes. Warning, also removes input/output due to large size.

# Declaration to make these phoney targets, means not dependent on files.
.PHONY: all clean

# Dirs for src and build artifacts. There must be a demo directory to build exes.
SRC_DIR=.
EXE_DIR=./demo

# Commands and Flags, cflags + f_debug are for gcc flags. Include and LD_Flags are where to find headers and libraries.
CC:=mpicc
CXX:=mpiCC
RM:=@rm -rf
CFLAGS:=-std=c99
CXXFLAGS:=
F_DEBUG:=-ggdb -Wall -Wextra -Winline -pedantic
INCLUDE:=-I./cunit/include 
LD_PATH:=-L./cunit/lib

# Library stuff, reduce duplication. Makes an archive of common code.
AR:=ar -rf
RANLIB:=ranlib
LIB_NAME:=shared.a #Name only.
LIB_ARC:=$(EXE_DIR)/$(LIB_NAME) #Final directory + name.
LIB_OBJS:=lib_f.o # Objects required.
LIBS:=$(LIB_ARC) -lcunit 

# Generic files to clean.
FILES_TO_CLEAN:=*.o *.a *.out *.exe *~ temp.* *.gcov *.gcda *.gcno

# Target executables to build
EXES = \
	$(EXE_DIR)/serial \
	$(EXE_DIR)/test_lib \
	$(EXE_DIR)/random \
	$(EXE_DIR)/parallel \
	$(EXE_DIR)/path_creator \

# Custom rules, define how to link and compile respectively.
$(EXE_DIR)/%: %.o 
	$(CC) -o $@ $(LD_PATH) $< $(LIBS) 

%.o: $(SRC_DIR)/%.c
	$(CC) $(INCLUDE) $(CFLAGS) $(F_DEBUG) -o $@ -c $< 

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(F_DEBUG) -o $@ -c $< 

# Rules to build and clean
all: $(LIB_ARC) $(EXES)
	$(RM) path_creator.o

$(EXE_DIR)/path_creator : path_creator.o
	$(CXX) -o $@ $(LD_PATH) $< 

$(LIB_ARC): $(LIB_OBJS)
	$(AR) $@ $^ 
	$(RANLIB) $@
	$(RM) $^	

clean:
	$(RM) $(EXES) $(LIB_ARC) core.* output.txt* log* $(FILES_TO_CLEAN)          
