
#include <marble/hash.h>
#include <marble/types.h>

#include <stdlib.h>
#include <string.h>

#define MHASH_ALPHA_MAX      0.75
#define MHASH_DEFAULT_SZ     32
#define MHASH_DEFAULT_KEY_SZ 32
#define MHASH_FNV_U64_PRIME  0x00000100000001B3
#define MHASH_FNV_U64_OFFSET 0xcbf29ce484222325

typedef struct _mhash_pair {
    char* key;
    void* val;
} _mhash_pair;

typedef struct _mhash {
    _mhash_pair* innards;
    mhash_free_fn free;
    mhash_deep_copy_fn copy;
    size_t key_size;
    size_t capacity;
    size_t size;
} _mhash;

mhash mhash_init(mhash_cfg cfg) {
    mhash table = malloc(sizeof (*table));
    _mhash_pair* innards = calloc(cfg.init_table_size, sizeof (_mhash_pair));
    table->innards = innards;
    table->free = cfg.free;
    table->copy = cfg.copy;
    table->key_size = cfg.key_size == 0 ? MHASH_DEFAULT_KEY_SZ : cfg.key_size;
    table->capacity = cfg.init_table_size == 0 ? MHASH_DEFAULT_SZ : cfg.init_table_size;
    table->size = 0;
    return table;
}

u64 _mhash_fn(char* key) {
    u64 hash = MHASH_FNV_U64_OFFSET;
    char* it_key = key;
    while (*it_key) {
        hash *= MHASH_FNV_U64_PRIME;
        hash ^= *++it_key;
    }
    return hash;
}

size_t _mhash_idx(mhash table, char* key) {
    size_t idx = _mhash_fn(key) % table->capacity;
     while (table->innards[idx].val != NULL && \
            strncmp(table->innards[idx].key, key, table->key_size) != 0) {
        idx++;
    }
    return idx;
}

void _mhash_resize(mhash table) {
    _mhash_pair* old_innards = table->innards;
    size_t old_capacity = table->capacity;
    table->innards = calloc(table->capacity*2, sizeof (_mhash_pair));
    table->capacity *= 2;
    for (size_t i = 0; i < old_capacity; i++) {
        mhash_put(table, old_innards[i].key, old_innards[i].val);
        free(old_innards[i].key);
    }
    if (table->free != NULL) {
        for (size_t i = 0; i < old_capacity; i++) {
            table->free(old_innards[i].val);
        }
    } 
    free(old_innards);
}

void mhash_put(mhash table, char* key, void* val) {
    if (key == NULL || val == NULL || strlen(key) >= table->key_size)
        return;

    if ((f64)(table->size + 1) / table->capacity >= MHASH_ALPHA_MAX)
        _mhash_resize(table);

    size_t idx = _mhash_idx(table, key);

    char* new_key = calloc(table->key_size, sizeof (char));
    strncpy(new_key, key, table->key_size);

    void* new_val;
    if (table->copy != NULL)
        new_val = table->copy(val);
    else
        new_val = val;

    table->innards[idx] = (_mhash_pair) {
        .key = new_key,
        .val = new_val
    };
    table->size++;
}

void* mhash_get(mhash table, char* key) {
    if (key == NULL || table->size == 0)
        return NULL;
    size_t idx = _mhash_idx(table, key);
    return table->innards[idx].val;
}

void mhash_del(mhash table, char* key) {
    if (table->size == 0)
        return;
    size_t idx = _mhash_idx(table, key);
    free(table->innards[idx].key);
    if (table->free != NULL)
        table->free(table->innards[idx].val);
    table->innards[idx].key = NULL;
    table->innards[idx].val = NULL;
    table->size--;
}

void mhash_map(mhash table, mhash_map_fn map_fn) {
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->innards[i].val != NULL)
            map_fn(table->innards[i].val);
    }
}

void mhash_free(mhash table) {
    if (table->size == 0) {
        free(table->innards);
        free(table);
        return;
    }
    if (table->free != NULL)
        mhash_map(table, table->free);
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->innards[i].key != NULL)
            free(table->innards[i].key);
    }
    free(table->innards);
    free(table);
}

