
SRCS=$(wildcard src/*.c)
INCS=$(wildcard include/*.h)

.PHONY: all
all: release debug

.PHONY: release
release: lib/libmarble.so

.PHONY: debug
debug: debug/libmarble.so

lib/libmarble.so lib/libmarble.a: ${SRCS} ${INCS} | compile_commands.json
	mkdir -p lib
	cd lib; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang ..; make

debug/libmarble.so debug/libmarble.a: ${SRCS} ${INCS} | compile_commands.json
	mkdir -p debug
	cd debug; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang ..; make

compile_commands.json: ${SRCS} ${INCS}
	CMAKE_EXPORT_COMPILE_COMMANDS=true cmake -DCMAKE_C_COMPILER=clang -S . -G "Unix Makefiles" -B cmake
	mv cmake/compile_commands.json .
	rm -rf cmake

.PHONY: docs
docs:
	mkdir -p docs
	doxygen

.PHONY: static
static:
	scan-build -analyze-headers -o test/static make debug

.PHONY: ready
ready:
	mkdir -p tmp
	mv lib/*.so tmp
	mv lib/*.a tmp
	rm -rf lib/*
	mv tmp/* lib
	rm -rf tmp

.PHONY: clean
clean:
	rm -rf docs/*
	rm -rf lib/*
	rm -rf debug/*

