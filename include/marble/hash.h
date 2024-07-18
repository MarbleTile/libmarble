
#ifndef MARBLE_HASH_H
#define MARBLE_HASH_H

#include <marble/types.h>

#include <stdbool.h>

/**
 *
 */
typedef struct _mhash* mhash;

typedef void (*mhash_map_fn) (void* value);

typedef void (*mhash_free_fn) (void* value);

typedef void* (*mhash_deep_copy_fn) (void* value);

typedef struct {
    u32 key_size;
    u32 init_table_size;
    mhash_free_fn free;
    mhash_deep_copy_fn copy;
} mhash_cfg;

mhash mhash_init(mhash_cfg config);

void mhash_put(mhash table, char* key, void* value);

void* mhash_get(mhash table, char* key);

void mhash_del(mhash table, char* key);

void mhash_map(mhash table, mhash_map_fn map_fn);

void mhash_free(mhash table);

#endif // MARBLE_HASH_H

