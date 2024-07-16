
.PHONY: all
all: release debug

.PHONY: release
release: src/* include/* compile_commands.json
	mkdir -p lib
	cd lib; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang ..; make

.PHONY: debug
debug: src/* include/* compile_commands.json
	mkdir -p debug
	cd debug; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang ..; make

compile_commands.json: src/* include/* CMakeLists.txt
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

.PHONY: clean
clean:
	mkdir -p tmp
	mv lib/*.so tmp
	mv lib/*.a tmp
	rm -rf lib/*
	mv tmp/* lib
	rm -rf tmp
	rm -rf docs/*

