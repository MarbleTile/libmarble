
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <marble_types.h>
#include <marble_list.h>
#include <string.h>

void u32_print(void* data) {
    u32 v = *((u32*) data);
    printf("%"PRIu32"\n", v);
}

bool u32_filter(void* in) {
    return (*(u32*) in) < 3;
}

int u32_main(void) {

    mlist_cfg cfg = {0};
    mlist list = mlist_init(cfg);

    u32 a = 1;
    u32 b = 2;
    u32 c = 3;

    mlist_push_back(list, &a);
    mlist_push_back(list, &b);
    mlist_push_back(list, &c);


    mlist_map(list, u32_print);

    mlist new_list = mlist_filter(list, u32_filter);

    mlist_map(new_list, u32_print);

    mlist_free(list);

    mlist_map(new_list, u32_print);
    mlist_free(new_list);
    
    return 0;
}

void str_print(void* data) {
    printf("%s\n", (char*) data);
}

void* str_copy(void* data) {
    size_t sz = strlen((char*) data);
    char* out = calloc(sz+1, sizeof (char));
    strcpy(out, data);
    return out;
}

void str_free(void* data) {
    free(data);
}

void* str_reduce(void* d1, void* d2) {
    size_t sz = strlen(d1) + strlen(d2);
    char* out = calloc(sz+1, sizeof (char));
    strcat(out, d1);
    strcat(out, d2);
    return out;
}

//int str_main(void) {
int main(void) {

    mlist_cfg cfg = {
        .copy = str_copy,
        .free = str_free
    };
    mlist list = mlist_init(cfg);

    mlist_push_back(list, "test0");
    mlist_push_back(list, "test1");
    mlist_push_back(list, "test2");

    mlist_map(list, str_print);
    printf("---\n");

    char* out = mlist_reduce(list, str_reduce);

    printf("%s\n", out);

    free(out);

    mlist_free(list);
}

