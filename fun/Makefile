# Make file for main project.
# A few variables
include ../Make.defines

LIBS+=-lmpi

SRC_DIR=.
EXE_DIR=./demo

EXES = \
	${EXE_DIR}/first \
	${EXE_DIR}/send \
	${EXE_DIR}/sendAdv \
	${EXE_DIR}/oddEvenSort \

${EXE_DIR}/%: %.o 
	${CC} ${LIBS} -o $@ $< 

%.o: ${SRC_DIR}/%.c
	${CC} $(CFLAGS) $(F_DEBUG) -o $@ -c $< 

all: ${EXES}

clean:
	$(RM) $(EXES) $(FILES_TO_CLEAN)          
