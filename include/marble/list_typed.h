
#include <stdbool.h>

#include <marble/macro.h>
#include <marble/types.h>


/*****************************************************************************
 *                                  USAGE                                    *
 *---------------------------------------------------------------------------*
 * 1. define MLIST_TYPE to desired mlist type                                *
 * 2. if MLIST_TYPE is complicated and needs its own heap allocations,       *
 *      define MLIST_TYPE_IS_HEAP and pass your deep copy and free functions *
 *      to mlist_cfg when initializing an mlist                              *
 * 3. if using some pointer type, give it a type alias so renaming can work  *
 *      properly (e.g. typedef char* str)                                    *
 *****************************************************************************/

#ifdef MLIST_TYPE

#define MLIST_TYPE_NAME(name) MJOIN2(name, MLIST_TYPE)

#include <stdlib.h>
#include <string.h>

typedef struct MLIST_TYPE_NAME(_mlist)* MLIST_TYPE_NAME(mlist);

typedef bool (*MLIST_TYPE_NAME(mlist_find_fn)) (MLIST_TYPE data);

typedef void (*MLIST_TYPE_NAME(mlist_map_fn)) (MLIST_TYPE data);

typedef bool (*MLIST_TYPE_NAME(mlist_filter_fn)) (MLIST_TYPE data);

typedef MLIST_TYPE (*MLIST_TYPE_NAME(mlist_reduce_fn)) (MLIST_TYPE first, MLIST_TYPE second);

typedef void (*MLIST_TYPE_NAME(mlist_free_fn)) (MLIST_TYPE data);

typedef MLIST_TYPE (*MLIST_TYPE_NAME(mlist_deep_copy_fn)) (MLIST_TYPE data);

typedef struct MLIST_TYPE_NAME(mlist_cfg) {
    MLIST_TYPE_NAME(mlist_free_fn) free;
    MLIST_TYPE_NAME(mlist_deep_copy_fn) copy;
} MLIST_TYPE_NAME(mlist_cfg);

typedef struct MLIST_TYPE_NAME(_mlist_node) {
    struct MLIST_TYPE_NAME(_mlist_node)* next;
    struct MLIST_TYPE_NAME(_mlist_node)* prev;
    MLIST_TYPE data;
} MLIST_TYPE_NAME(_mlist_node);

typedef struct MLIST_TYPE_NAME(_mlist) {
    MLIST_TYPE_NAME(_mlist_node)* begin;
    MLIST_TYPE_NAME(_mlist_node)* end;
    size_t length;
    u32 data_size;
    MLIST_TYPE_NAME(mlist_free_fn) free;
    MLIST_TYPE_NAME(mlist_deep_copy_fn) copy;
} MLIST_TYPE_NAME(_mlist);

MLIST_TYPE_NAME(mlist) MLIST_TYPE_NAME(mlist_init)(MLIST_TYPE_NAME(mlist_cfg) cfg) {
    MLIST_TYPE_NAME(mlist) list = malloc(sizeof (*list));
    list->begin = NULL;
    list->end = NULL;
    list->length = 0;
    if (cfg.free == NULL || cfg.copy == NULL) {
        list->free = NULL;
        list->copy = NULL;
    } else {
        list->free = cfg.free;
        list->copy = cfg.copy;
    }
    return list;
}



MLIST_TYPE MLIST_TYPE_NAME(mlist_peek_back)(MLIST_TYPE_NAME(mlist) list) {
    if (list->length == 0)
        return NULL;
    return list->end->data;
}

MLIST_TYPE MLIST_TYPE_NAME(mlist_peek_front)(MLIST_TYPE_NAME(mlist) list) {
    if (list->length == 0)
        return NULL;
    return list->begin->data;
}

void MLIST_TYPE_NAME(_mlist_push_first)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE data) {
    list->begin = malloc(sizeof (MLIST_TYPE_NAME(_mlist_node)));
    list->begin->next = NULL;
    list->begin->prev = NULL;
#ifdef MLIST_TYPE_IS_HEAP
    list->begin->data = list->copy(data);
#else
    list->begin->data = data;
#endif
    list->end = list->begin;
    list->length++;
}

void MLIST_TYPE_NAME(mlist_push_back)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE data) {
    if (list->length == 0) {
        MLIST_TYPE_NAME(_mlist_push_first)(list, data);
    } else {
        MLIST_TYPE_NAME(_mlist_node)* new_end = malloc(sizeof (MLIST_TYPE_NAME(_mlist_node)));
        new_end->next = NULL;
        new_end->prev = list->end;
        list->end->next = new_end;
#ifdef MLIST_TYPE_IS_HEAP
        new_end->data = list->copy(data);
#else
        new_end->data = data;
#endif
        list->end = new_end;
        list->length++;
    }
}

void MLIST_TYPE_NAME(mlist_push_front)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE data) {
    if (list->length == 0) {
        MLIST_TYPE_NAME(_mlist_push_first)(list, data);
    } else {
        MLIST_TYPE_NAME(_mlist_node)* new_begin = malloc(sizeof (MLIST_TYPE_NAME(_mlist_node)));
        new_begin->next = list->begin;
        new_begin->prev = NULL;
        list->begin->prev = new_begin;
#ifdef MLIST_TYPE_IS_HEAP
        new_begin->data = list->copy(data);
#else
        new_begin->data = data;
#endif
        list->begin = new_begin;
        list->length++;
    }
}

MLIST_TYPE MLIST_TYPE_NAME(_mlist_pop_node)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE_NAME(_mlist_node)* node) {
    MLIST_TYPE out = node->data;
#ifdef MLIST_TYPE_IS_HEAP
    out = list->copy(node->data);
    list->free(node->data);
#endif
    free(node);
    list->length--;
    return out;
}

MLIST_TYPE MLIST_TYPE_NAME(_mlist_pop_first)(MLIST_TYPE_NAME(mlist) list) {
    MLIST_TYPE out = MLIST_TYPE_NAME(_mlist_pop_node)(list, list->end);
    list->length = 0;
    list->begin = NULL;
    list->end = NULL;
    return out;
}

MLIST_TYPE MLIST_TYPE_NAME(mlist_pop_back)(MLIST_TYPE_NAME(mlist) list) {
    if (list->length == 0) {
        return NULL;
    } else if (list->length == 1) {
        return MLIST_TYPE_NAME(_mlist_pop_first)(list);
    } else {
        MLIST_TYPE_NAME(_mlist_node)* new_end = list->end->prev;
        new_end->next = NULL;
        MLIST_TYPE out = MLIST_TYPE_NAME(_mlist_pop_node)(list, list->end);
        list->end = new_end;
        return out;
    }
}
      
MLIST_TYPE MLIST_TYPE_NAME(mlist_pop_front)(MLIST_TYPE_NAME(mlist) list) {
    if (list->length == 0) {
        return NULL;
    } else if (list->length == 1) {
        return MLIST_TYPE_NAME(_mlist_pop_first)(list);
    } else {
        MLIST_TYPE_NAME(_mlist_node)* new_begin = list->begin->next;
        new_begin->prev = NULL;
        MLIST_TYPE out = MLIST_TYPE_NAME(_mlist_pop_node)(list, list->begin);
        list->begin = new_begin;
        return out;
    }
}
      
bool MLIST_TYPE_NAME(mlist_contains)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE_NAME(mlist_find_fn) find_fn) {
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin;
    while (curr != NULL) {
        if (find_fn(curr->data))
            return true;
        curr = curr->next;
    }
    return false;
}

void MLIST_TYPE_NAME(mlist_map)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE_NAME(mlist_map_fn) map_fn) {
    if (list->length == 0)
        return;
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin;
    while (curr != NULL) {
        map_fn(curr->data);
        curr = curr->next;
    }
}

MLIST_TYPE_NAME(mlist) MLIST_TYPE_NAME(mlist_filter)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE_NAME(mlist_filter_fn) filter_fn) {
    if (list->length == 0)
        return NULL;
    MLIST_TYPE_NAME(mlist_cfg) new_cfg = {
        .copy = list->copy,
        .free = list->free,
    };
    MLIST_TYPE_NAME(mlist) new_list = MLIST_TYPE_NAME(mlist_init)(new_cfg);
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin;
    while (curr != NULL) {
        if (filter_fn(curr->data)) 
            MLIST_TYPE_NAME(mlist_push_back)(new_list, curr->data);
        curr = curr->next;
    }
    return new_list;
}

MLIST_TYPE MLIST_TYPE_NAME(mlist_reduce)(MLIST_TYPE_NAME(mlist) list, MLIST_TYPE_NAME(mlist_reduce_fn) reduce_fn) {
    if (list->length < 2)
        return NULL;
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin->next;
    MLIST_TYPE accum;
#ifdef MLIST_TYPE_IS_HEAP
    accum = list->copy(list->begin->data);
    MLIST_TYPE tmp = NULL;
    while (curr != NULL) {
        tmp = accum;
        accum = reduce_fn(tmp, curr->data);
        list->free(tmp);
        curr = curr->next;
#else
    accum = list->begin->data;
    while (curr != NULL) {
        accum = reduce_fn(accum, curr->data);
        curr = curr->next;
#endif
    }
    return accum;
}

size_t MLIST_TYPE_NAME(mlist_length)(MLIST_TYPE_NAME(mlist) list) {
    return list->length;
}

MLIST_TYPE_NAME(mlist) MLIST_TYPE_NAME(mlist_copy)(MLIST_TYPE_NAME(mlist) list) {
    MLIST_TYPE_NAME(mlist_cfg) new_cfg = {
        .copy = list->copy,
        .free = list->free
    };
    MLIST_TYPE_NAME(mlist) new_list = MLIST_TYPE_NAME(mlist_init)(new_cfg);
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin;
    while (curr != NULL) {
        MLIST_TYPE_NAME(mlist_push_back)(new_list, curr->data);
        curr = curr->next;
    }
    return new_list;
}

void MLIST_TYPE_NAME(mlist_free)(MLIST_TYPE_NAME(mlist) list) {
    if (list->length == 0) {
        free(list);
        return;
    }
    MLIST_TYPE_NAME(_mlist_node)* curr = list->begin;
    MLIST_TYPE_NAME(_mlist_node)* next = NULL;
#ifdef MLIST_TYPE_IS_HEAP
    MLIST_TYPE_NAME(mlist_map)(list, list->free);
#endif
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

#undef MLIST_TYPE
#undef MLIST_TYPE_IS_HEAP

#endif

