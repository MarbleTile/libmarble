
# libmarble

A utility library for MarbleTile's personal C projects.

## building

* `make release` builds static and dynamic in `./lib`
* `make debug` builds with debug symbols in `./debug`
* `make docs` makes HTML and Latex documentation in `./docs`
* do the usual `cmake` thing if you don't want to use clang

### marble.h

Everything. All of it.

### marble\_types.h

Some type aliases I like.

### marble\_list.h

Provides `mlist`, a doubly linked list implementing much of C++ `std::list` 
behavior, as well as Python's `map()`, `filter()`, and `reduce()`.

### marble\_list\_typed.h

A set of macros to generate `mlist_T` for a more pleasurable experience
when working with built-in C types.

### marble\_hash.h

Provides `mhash`, a hash table.

### marble\_hash\_typed.h

A set of macros to generate `mhash_T`.

