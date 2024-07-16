
CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -Werror
MARBLE_PATH=../..
LFLAGS=-I${MARBLE_PATH}/include -L${MARBLE_PATH}/debug -lmarble

test: test.c ${MARBLE_PATH}/debug/libmarble.so
	${CC} ${CFLAGS} -g -o $@ test.c ${LFLAGS}

.PHONY: run
run: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug ./test

.PHONY: memcheck
memcheck: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug valgrind --leak-check=full ./test

.PHONY: debug
debug: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug gdb ./test

${MARBLE_PATH}/debug/libmarble.so: ${MARBLE_PATH}/src/* ${MARBLE_PATH}/include/* 
	$(MAKE) -C ..

