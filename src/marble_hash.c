
#include <marble/hash.h>
#include <marble/types.h>

#include <stdlib.h>

#define HASH_ALPHA_MAX 0.75f

typedef struct _mhash_pair {
    char* key;
    void* val;
} _mhash_pair;

typedef struct _mhash {
    _mhash_pair* innards;
    u32 key_size;
    u32 capacity;
    u32 size;
} _mhash;

u32 mhash_fn(const char* k);

mhash mhash_init(const mhash_cfg cfg) {
    mhash table = malloc(sizeof (*table));
    _mhash_pair* innards = calloc(cfg.init_table_size, sizeof (_mhash_pair));
    table->innards = innards;
    table->key_size = cfg.key_size;
    table->capacity = cfg.init_table_size;
    table->size = 0;
    return table;
}

void _mhash_resize(mhash table);

void mhash_put(mhash table, const char* k, void* v);

void* mhash_get(mhash table, const char* k);

void mhash_del(mhash table, const char* k);

void mhash_print(mhash table);

void mhash_free(mhash table);

