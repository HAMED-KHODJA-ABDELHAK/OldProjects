# Makefile for JustForFun Files

# A few variables
include ../Make.defines

LIBS+=-lpanel -lncurses

SRC_DIR=.
EXE_DIR=./demo

EXES = \
    ${EXE_DIR}/first_panel \
    ${EXE_DIR}/panel_browse \

${EXE_DIR}/%: %.o
	${CC} -o $@ $< ${LIBS}

%.o: ${SRC_DIR}/%.c
	${CC} -o $@ -c $< $(CFLAGS) $(F_DEBUG) 

all: ${EXES}

clean:
	$(RM) ${EXES}
