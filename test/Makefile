
CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -Werror
MARBLE_PATH=..
LFLAGS=-I${MARBLE_PATH}/include -L${MARBLE_PATH}/debug -lmarble

test: test.c
	${CC} ${CFLAGS} -g -o $@ $^ ${LFLAGS}

.PHONY: run
run: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug ./test

.PHONY: memcheck
memcheck: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug valgrind --leak-check=full ./test

.PHONY: debug
debug: test
	LD_LIBRARY_PATH=${MARBLE_PATH}/debug gdb ./test

