
/*****************************************************************************
 *                                  USAGE                                    *
 *---------------------------------------------------------------------------*
 * 1. define MHASH_TYPE to desired mhash type                                *
 * 2. if MHASH_TYPE is complicated and needs its own heap allocations,       *
 *      define MHASH_TYPE_IS_DYNAMIC and pass your deep copy and free        *
 *      functions to mhash_cfg when initializing an mhash                    *
 * 3. if using some pointer type, give it a type alias so renaming can work  *
 *      properly (e.g. typedef char* str)                                    *
 *****************************************************************************/

// TEMPORARY
#define MHASH_TYPE u32

#ifdef MHASH_TYPE

#include <marble/macro.h>
#include <marble/types.h>

#include <stdlib.h>
#include <string.h>

#ifndef MHASH_ALPHA_MAX
#define MHASH_ALPHA_MAX 0.75
#endif

#ifndef MHASH_DEFAULT_SZ
#define MHASH_DEFAULT_SZ 32
#endif

#ifndef MHASH_DEFAULT_KEY_SZ
#define MHASH_DEFAULT_KEY_SZ 32
#endif

#ifndef MHASH_FNV_U64_PRIME 
#define MHASH_FNV_U64_PRIME 0x00000100000001B3
#endif

#ifndef MHASH_FNV_U64_OFFSET
#define MHASH_FNV_U64_OFFSET 0xcbf29ce484222325
#endif

#ifndef MHASH_TYPE_NAME
#define MHASH_TYPE_NAME(name) MJOIN2(name, MHASH_TYPE)
#endif

#ifndef MHASH_TYPE_0
#define MHASH_TYPE_0 (MHASH_TYPE) {0}
#endif

typedef struct MHASH_TYPE_NAME(_mhash)* MHASH_TYPE_NAME(mhash);

typedef void (*MHASH_TYPE_NAME(mhash_map_fn)) (MHASH_TYPE value);

typedef void (*MHASH_TYPE_NAME(mhash_free_fn)) (MHASH_TYPE value);

typedef MHASH_TYPE (*MHASH_TYPE_NAME(mhash_deep_copy_fn)) (MHASH_TYPE value);

typedef struct {
    u32 key_size;
    u32 init_table_size;
    MHASH_TYPE_NAME(mhash_free_fn) free;
    MHASH_TYPE_NAME(mhash_deep_copy_fn) copy;
} MHASH_TYPE_NAME(mhash_cfg);

typedef struct MHASH_TYPE_NAME(_mhash_pair) {
    char* key;
    MHASH_TYPE val;
} MHASH_TYPE_NAME(_mhash_pair);

typedef struct MHASH_TYPE_NAME(_mhash) {
    MHASH_TYPE_NAME(_mhash_pair)* innards;
    MHASH_TYPE_NAME(mhash_free_fn) free;
    MHASH_TYPE_NAME(mhash_deep_copy_fn) copy;
    size_t key_size;
    size_t capacity;
    size_t size;
} MHASH_TYPE_NAME(_mhash);

MHASH_TYPE_NAME(mhash) MHASH_TYPE_NAME(mhash_init)(MHASH_TYPE_NAME(mhash_cfg) cfg) {
    MHASH_TYPE_NAME(mhash) table = malloc(sizeof (*table));
    MHASH_TYPE_NAME(_mhash_pair)* innards = calloc(cfg.init_table_size, sizeof (MHASH_TYPE_NAME(_mhash_pair)));
    table->innards = innards;
    table->free = cfg.free;
    table->copy = cfg.copy;
    table->key_size = cfg.key_size == 0 ? MHASH_DEFAULT_KEY_SZ : cfg.key_size;
    table->capacity = cfg.init_table_size == 0 ? MHASH_DEFAULT_SZ : cfg.init_table_size;
    table->size = 0;
    return table;
}

u64 MHASH_TYPE_NAME(_mhash_fn)(char* key) {
    u64 hash = MHASH_FNV_U64_OFFSET;
    char* it_key = key;
    while (*it_key) {
        hash *= MHASH_FNV_U64_PRIME;
        hash ^= *++it_key;
    }
    return hash;
}

// SEGFAULTING HERE???
size_t MHASH_TYPE_NAME(_mhash_idx)(MHASH_TYPE_NAME(mhash) table, char* key) {
    size_t idx = MHASH_TYPE_NAME(_mhash_fn)(key) % table->capacity;
    while (table->innards[idx].key != NULL && \
            strncmp(table->innards[idx].key, key, table->key_size) != 0) {
        idx++;
    }
    return idx;
}

void MHASH_TYPE_NAME(mhash_put)(MHASH_TYPE_NAME(mhash) table, char* key, MHASH_TYPE val);

void MHASH_TYPE_NAME(_mhash_resize)(MHASH_TYPE_NAME(mhash) table) {
    MHASH_TYPE_NAME(_mhash_pair)* old_innards = table->innards;
    size_t old_capacity = table->capacity;
    table->innards = calloc(table->capacity*2, sizeof (MHASH_TYPE_NAME(_mhash_pair)));
    table->capacity *= 2;
    for (size_t i = 0; i < old_capacity; i++) {
        MHASH_TYPE_NAME(mhash_put)(table, old_innards[i].key, old_innards[i].val);
        free(old_innards[i].key);
    }
    if (table->free != NULL) {
        for (size_t i = 0; i < old_capacity; i++) {
            table->free(old_innards[i].val);
        }
    } 
    free(old_innards);
}

void MHASH_TYPE_NAME(mhash_put)(MHASH_TYPE_NAME(mhash) table, char* key, MHASH_TYPE val) {
#ifdef MHASH_TYPE_IS_DYNAMIC
    if (key == NULL || val == NULL || strlen(key) >= table->key_size)
        return;
#else
    if (key == NULL || strlen(key) >= table->key_size)
        return;
#endif

    if ((f64)(table->size + 1) / table->capacity >= MHASH_ALPHA_MAX)
        MHASH_TYPE_NAME(_mhash_resize)(table);

    size_t idx = MHASH_TYPE_NAME(_mhash_idx)(table, key);

    char* new_key = calloc(table->key_size, sizeof (char));
    strncpy(new_key, key, table->key_size);

    MHASH_TYPE new_val;
#ifdef MHASH_TYPE_IS_DYNAMIC
        new_val = table->copy(val);
#else
        new_val = val;
#endif

    table->innards[idx] = (MHASH_TYPE_NAME(_mhash_pair)) {
        .key = new_key,
        .val = new_val
    };
    table->size++;
}

MHASH_TYPE MHASH_TYPE_NAME(mhash_get)(MHASH_TYPE_NAME(mhash) table, char* key) {
    if (key == NULL || table->size == 0)
#ifdef MHASH_TYPE_IS_DYNAMIC
        return NULL;
#else
        return MHASH_TYPE_0;
#endif
    size_t idx = MHASH_TYPE_NAME(_mhash_idx)(table, key);
    return table->innards[idx].val;
}

void MHASH_TYPE_NAME(mhash_del)(MHASH_TYPE_NAME(mhash) table, char* key) {
    if (table->size == 0)
        return;
    size_t idx = MHASH_TYPE_NAME(_mhash_idx)(table, key);
    free(table->innards[idx].key);
    table->innards[idx].key = NULL;
#ifdef MHASH_TYPE_IS_DYNAMIC
    table->free(table->innards[idx].val);
    table->innards[idx].val = NULL;
#else
    table->innards[idx].val = MHASH_TYPE_0;
#endif
    table->size--;
}

void MHASH_TYPE_NAME(mhash_map)(MHASH_TYPE_NAME(mhash) table, MHASH_TYPE_NAME(mhash_map_fn) map_fn) {
    for (size_t i = 0; i < table->capacity; i++) {
#ifdef MHASH_TYPE_IS_DYNAMIC
        if (table->innards[i].val != NULL)
#endif
            map_fn(table->innards[i].val);
    }
}

void MHASH_TYPE_NAME(mhash_free)(MHASH_TYPE_NAME(mhash) table) {
    if (table->size == 0) {
        free(table->innards);
        free(table);
        return;
    }
#ifdef MHASH_TYPE_IS_DYNAMIC
    MHASH_TYPE_NAME(mhash_map)(table, table->free);
#endif
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->innards[i].key != NULL)
            free(table->innards[i].key);
    }
    free(table->innards);
    free(table);
}

#endif

