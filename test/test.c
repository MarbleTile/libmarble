
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <marble_types.h>
#include <marble_list.h>

//void print_list(void* data) {
//    u32 v = *((u32*)data);
//    printf("%"PRIu32"\n", v);
//}

void print_list(void* data) {
    u32 v = *((u32*) data);
    printf("%"PRIu32"\n", v);
}

void u32_free(void* in) {
    free(in);
}

void* u32_copy(void* in) {
    u32* out = malloc(sizeof (*out));
    *out = *(u32*)in;
    return out;
}

bool u32_filter(void* in) {
    return (*(u32*) in) < 3;
}

int main(void) {

    mlist_cfg cfg = {0};
    mlist list = mlist_init(cfg);

    u32 a = 1;
    u32 b = 2;
    u32 c = 3;

    mlist_push_back(list, &a);
    mlist_push_back(list, &b);
    mlist_push_back(list, &c);


    mlist_map(list, print_list);

    mlist new_list = mlist_filter(list, u32_filter);

    mlist_map(new_list, print_list);

    mlist_free(list);

    mlist_map(new_list, print_list);
    mlist_free(new_list);
    
    return 0;
}

