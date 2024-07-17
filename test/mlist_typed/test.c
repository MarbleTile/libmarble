
#include <stdio.h>
#include <string.h>

#include <marble/types.h>

#define MLIST_TYPE u32
#include <marble/list_typed.h>

typedef char* str;
#define MLIST_TYPE str
#define MLIST_TYPE_IS_HEAP
#include <marble/list_typed.h>

void u32_print(u32 data) {
    printf("%u\n", data);
}

void u32_print_list(mlist_u32 list) {
    mlist_map_u32(list, u32_print);
    printf("---\n");
}

char* str_copy(char* data) {
    size_t sz = strlen(data) + 1;
    char* out = malloc(sizeof (char) * sz);
    strcpy(out, data);
    return out;
}

void str_free(char* data) {
    free(data);
}

void str_print(char* data) {
    printf("%s\n", data);
}

void str_print_list(mlist_str list) {
    mlist_map_str(list, str_print);
    printf("---\n");
}

char* str_reduce(char* accum, char* next) {
    size_t sz = strlen(accum) + strlen(next) + 1;
    char* out = malloc(sz * sizeof (char));
    strcat(out, accum);
    strcat(out, next);
    return out;
}

int main(void) {

    mlist_cfg_u32 cfg_u32 = {0};

    mlist_u32 list_u32 = mlist_init_u32(cfg_u32);

    mlist_push_back_u32(list_u32, 1);
    mlist_push_back_u32(list_u32, 2);
    mlist_push_back_u32(list_u32, 3);
    mlist_push_back_u32(list_u32, 4);
    mlist_push_back_u32(list_u32, 5);

    u32_print_list(list_u32);

    mlist_free_u32(list_u32);

    mlist_cfg_str cfg_str = {
        .copy = str_copy,
        .free = str_free
    };

    mlist_str list_str = mlist_init_str(cfg_str);

    mlist_push_back_str(list_str, "hello");
    mlist_push_back_str(list_str, "world");
    mlist_push_back_str(list_str, "world");
    mlist_push_back_str(list_str, "world");
    mlist_push_back_str(list_str, "world");

    str_print_list(list_str);

    char* str_reduced = mlist_reduce_str(list_str, str_reduce);
    printf("%s\n", str_reduced);
    free(str_reduced);

    mlist_free_str(list_str);

    return 0;
}

