
#include <marble/list.h>
#include <marble/types.h>

#include <stdlib.h>
#include <string.h>

typedef struct _mlist_node {
    struct _mlist_node* next;
    struct _mlist_node* prev;
    void* data;
} _mlist_node;

typedef struct _mlist {
    _mlist_node* begin;
    _mlist_node* end;
    mlist_free_fn free;
    mlist_deep_copy_fn copy;
    size_t length;
    u32 data_size;
} _mlist;

mlist mlist_init(mlist_cfg cfg) {
    mlist list = malloc(sizeof (*list));
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

mlist mlist_copy(mlist list) {
    mlist_cfg new_cfg = {
        .copy = list->copy,
        .free = list->free
    };
    mlist new_list = mlist_init(new_cfg);
    _mlist_node* curr = list->begin;
    while (curr != NULL) {
        mlist_push_back(new_list, curr->data);
        curr = curr->next;
    }
    return new_list;
}

void* mlist_peek_back(mlist list) {
    if (list->length == 0)
        return NULL;
    return list->end->data;
}

void* mlist_peek_front(mlist list) {
    if (list->length == 0)
        return NULL;
    return list->begin->data;
}

void _mlist_set_data(mlist list, _mlist_node* node, void* data) {
    if (list->copy)
        node->data = list->copy(data);
    else
        node->data = data;
}

void _mlist_push_first(mlist list, void* data) {
    list->begin = malloc(sizeof (_mlist_node));
    list->begin->next = NULL;
    list->begin->prev = NULL;
    _mlist_set_data(list, list->begin, data);
    list->end = list->begin;
    list->length++;
}

void mlist_push_back(mlist list, void* data) {
    if (list->length == 0) {
        _mlist_push_first(list, data);
    } else {
        _mlist_node* new_end = malloc(sizeof (_mlist_node));
        new_end->next = NULL;
        new_end->prev = list->end;
        list->end->next = new_end;
        _mlist_set_data(list, new_end, data);
        list->end = new_end;
        list->length++;
    }
}

void mlist_push_front(mlist list, void* data) {
    if (list->length == 0) {
        _mlist_push_first(list, data);
    } else {
        _mlist_node* new_begin = malloc(sizeof (_mlist_node));
        new_begin->next = list->begin;
        new_begin->prev = NULL;
        list->begin->prev = new_begin;
        _mlist_set_data(list, new_begin, data);
        list->begin = new_begin;
        list->length++;
    }
}

void* _mlist_pop_node(mlist list, _mlist_node* node) {
    void* out = node->data;
    if (list->copy) {
        out = list->copy(node->data);
        list->free(node->data);
    }
    free(node);
    list->length--;
    return out;
}

void* _mlist_pop_first(mlist list) {
    void* out = _mlist_pop_node(list, list->end);
    list->length = 0;
    list->begin = NULL;
    list->end = NULL;
    return out;
}

void* mlist_pop_back(mlist list) {
    if (list->length == 0) {
        return NULL;
    } else if (list->length == 1) {
        return _mlist_pop_first(list);
    } else {
        _mlist_node* new_end = list->end->prev;
        new_end->next = NULL;
        void* out = _mlist_pop_node(list, list->end);
        list->end = new_end;
        return out;
    }
}
      
void* mlist_pop_front(mlist list) {
    if (list->length == 0) {
        return NULL;
    } else if (list->length == 1) {
        return _mlist_pop_first(list);
    } else {
        _mlist_node* new_begin = list->begin->next;
        new_begin->prev = NULL;
        void* out = _mlist_pop_node(list, list->begin);
        list->begin = new_begin;
        return out;
    }
}
      
bool mlist_contains(mlist list, mlist_find_fn find_fn) {
    _mlist_node* curr = list->begin;
    while (curr != NULL) {
        if (find_fn(curr->data))
            return true;
        curr = curr->next;
    }
    return false;
}

void mlist_map(mlist list, mlist_map_fn map_fn) {
    if (list->length == 0)
        return;
    _mlist_node* curr = list->begin;
    while (curr != NULL) {
        map_fn(curr->data);
        curr = curr->next;
    }
}

mlist mlist_filter(mlist list, mlist_filter_fn filter_fn) {
    if (list->length == 0)
        return NULL;
    mlist_cfg new_cfg = {
        .copy = list->copy,
        .free = list->free,
    };
    mlist new_list = mlist_init(new_cfg);
    _mlist_node* curr = list->begin;
    while (curr != NULL) {
        if (filter_fn(curr->data)) 
            mlist_push_back(new_list, curr->data);
        curr = curr->next;
    }
    return new_list;
}

void* mlist_reduce(mlist list, mlist_reduce_fn reduce_fn) {
    if (list->length < 2)
        return NULL;
    _mlist_node* curr = list->begin->next;
    void* accum;
    if (list->copy != NULL) {
        accum = list->copy(list->begin->data);
        void* tmp = NULL;
        while (curr != NULL) {
            tmp = accum;
            accum = reduce_fn(tmp, curr->data);
            list->free(tmp);
            curr = curr->next;
        }
    } else {
        accum = list->begin->data;
        while (curr != NULL) {
            accum = reduce_fn(accum, curr->data);
            curr = curr->next;
        }
    }
    return accum;
}

size_t mlist_length(mlist list) {
    return list->length;
}

void mlist_free(mlist list) {
    if (list->length == 0) {
        free(list);
        return;
    }
    _mlist_node* curr = list->begin;
    _mlist_node* next = NULL;
    if (list->free != NULL)
        mlist_map(list, list->free);
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

