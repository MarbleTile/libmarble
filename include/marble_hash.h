
#pragma once

#include "marble_types.h"

/**
 *
 */
typedef struct _mhash* mhash;
typedef struct {
    u32 key_size;
    u32 init_table_size;
} mhash_cfg;

u32     mhash_fn(const char* key);
mhash   mhash_init(const mhash_cfg config);
void    mhash_put(mhash table, const char* key, void* value);
void*   mhash_get(mhash table, const char* key);
void    mhash_print(mhash table);
void    mhash_free(mhash table);

