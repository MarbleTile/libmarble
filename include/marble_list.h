
#pragma once

#include <stdbool.h>

#include "marble_types.h"

//TODO: make thread-safe parallel versions of map, reduce, filter
//  COUNTERPOINT: perhaps just a different header altogether

/** A doubly linked list accepting generic types (`void*`).
 *
 * Inspired by C++ `std::list` and Python's map, reduce, filter
 */
typedef struct _mlist* mlist;

/** A type alias for the function expected by `mlist_find()`.
 *
 * Use with `mlist_find()` to return the first element that makes this 
 * function return true. Run for each element of the given `mlist`
 *
 * @param data An element of an `mlist`
 * @return Whether `data` can be considered found
 */
typedef bool (*mlist_find_fn) (void* data);

/** A type alias for the function expected by `mlist_map()`.
 *
 * Is run in sequential order for each element in an `mlist` when passed 
 * to `mlist_map()`.
 *
 * @param data An element of an `mlist`
 */
typedef void (*mlist_map_fn) (void* data);

/** A type alias for the function expected by `mlist_filter()`.
 *
 * Must return a boolean value. Use with `mlist_filter()` to create a new
 * `mlist` based on the elements that return true. Is run in sequential order
 * and creates a deep copy for each element passing the filter test. This
 * allows `mlist_free`ing the previous list.
 *
 * @param data An element of an `mlist`
 */
typedef bool (*mlist_filter_fn) (void* data);

/** A type alias for the function expected by `mlist_reduce()`.
 *
 * Use in conjunction with `mlist_reduce()` to reduce an `mlist` to a single
 * value based on this procedure's definition. Returns newly allocated
 * memory if `copy` and `free` are defined.
 *
 * @param first The first data element
 * @param second The data element after `first`
 * @return The value of combining `first` and `second`
 */
typedef void* (*mlist_reduce_fn) (void* first, void* second);

/** A type alias for the function expected by `mlist_free()`.
 *
 * Use with `mlist_free_custom()` to specify how list elements should be
 * freed. Called in a `mlist_map()` behind the scenes.
 *
 * @param data The element to be freed
 */
typedef void (*mlist_free_fn) (void* data);

/** A type alias for the function to create a deep copy of an `mlist` element.
 *
 * Used in `mlist_copy()` and `mlist_filter()`.
 *
 * @param data The element to be copied
 * @return A deep copy of the given element
 */
typedef void* (*mlist_deep_copy_fn) (void* data);

/** A type alias for the configuration expected by `mlist_init()`.
 *
 * `copy` and `free` may be NULL. If one is defined, the other must be, too.
 */
typedef struct mlist_cfg {
    mlist_free_fn free;
    mlist_deep_copy_fn copy;
} mlist_cfg;

/** Constructs a new `mlist`.
 *
 * @param cfg The configuration structure
 * @return A new `mlist`
 */
mlist mlist_init(mlist_cfg cfg);

/** Constructs a deep copy of an `mlist`.
 *
 * @param list The input list
 * @return A deep copy of the given list
 */
mlist mlist_copy(mlist list);

/**
 * Appends `data` to the end of `list`.
 *
 * @param list The input list
 * @param data The input data item
 */
void mlist_push_back(mlist list, void* data);

/**
 * Appends `data` to the front of `list`
 *
 * @param list The input list
 * @param data The input data item
 */
void mlist_push_front(mlist list, void* data);

/**
 * Removes the last element from `list`, returning it. If `copy` and `free`
 * are defined, then returns a deep copy of the element.
 *
 * @param list The input list
 * @return The last element of `list`
 */
void* mlist_pop_back(mlist list);
      
/**   
 * Removes the first element from `list`, returning it. If `copy` and `free`
 * are defined, then returns a deep copy of the element.
 *
 * @param list The input list
 * @return The first element of `list`
 */   
void* mlist_pop_front(mlist list);
      
/**   
 * Returns whether `list` contains an element meeting the criteria defined
 * by `find_fn`. A counterpart to `mlist_filter()` and `mlist_reduce()` for
 * this usecase.
 *
 * @param list The input list
 * @param find_fn The function governing what is to be found
 * @return Whether `list` contains the given element
 */   
bool mlist_contains(mlist list, mlist_find_fn find_fn);

/**   
 * Returns the `list` element at position `index`
 *
 * @param list The input list
 * @param index The index to find an element
 * @return The element at `index`
 */   
void* mlist_at(mlist list, size_t index);

/**
 * Runs `map_fn` for each element of `list`, sequentially.
 *
 * @param list The input list
 * @param map_fn The function to be run for each `list` element
 */
void mlist_map(mlist list, mlist_map_fn map_fn);

/**
 * Creates a new `mlist` containing deep copies of each element in `list`
 * that makes `filter_fn` return true.
 *
 * @param list The input list
 * @param filter_fn The function to be run for each `list` element
 * @return A new `mlist` that only has desired elements
 */
mlist mlist_filter(mlist list, mlist_filter_fn filter_fn);

/**
 * Reduces `list` to a single element value by some procedure defined
 * in `reduce_fn`. This value is newly allocated and must be freed. `list`
 * will be unchanged.
 * 
 * @param list The input list
 * @param reduce_fn The function to be run for all elements of `list`
 * @return The accumulated value
 */
void* mlist_reduce(mlist list, mlist_reduce_fn reduce_fn);

/**
 * Returns the length of `list`.
 *
 * @param list The input list
 */
size_t mlist_length(mlist list);

/**
 * Destroys `list`. Destroys each element therein.
 * @param list The input list
 */
void mlist_free(mlist list);

