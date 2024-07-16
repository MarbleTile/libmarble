
# libmarble

A utility library for my personal C projects.

## Building

* `make release` builds static and dynamic in `./lib`
* `make debug` builds with debug symbols in `./debug`
* `make docs` makes HTML and Latex documentation in `./docs`
* do the usual `cmake` thing if you don't want to use clang

## Headers

### libmarble.h

Includes everything but `*_typed.h`. 

### marble/types.h

Some type aliases I like.

### marble/macros.h

Useful macros.

### marble/list.h

Provides `mlist`, a doubly linked list implementing much of C++ `std::list` 
behavior, as well as Python's `map()`, `filter()`, and `reduce()`.

### marble/hash.h

Provides `mhash`, a hash table.

### marble/\*\_typed.h

Sets of macros to create type-"safe" versions of whatever ADT they correspond
to. Just define `*_TYPE` before including to generate the typed version. For
more complex types that require heap allocation, define `*_TYPE_IS_HEAP` as
well. Follows the same `*_cfg` methodology for accessing

## Credit
* C generics
    * [Sam Rebelsky](https://rebelsky.cs.grinnell.edu/musings/cnix-macros-generics)
    * [Ian Fisher](https://iafisher.com/blog/2020/06/type-safe-generics-in-c)
    * [John Kimble](https://codereview.stackexchange.com/questions/274860/concept-of-implementing-generic-types-in-c-using-macros)

Thank you for helping me understand C generics.

