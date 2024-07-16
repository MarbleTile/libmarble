
#include <stdio.h>

#include <marble/types.h>

#define MLIST_TYPE u32
#include <marble/list_typed.h>

void u32_print(u32 data) {
    printf("%u\n", data);
}

int main(void) {

    mlist_cfg_u32 cfg = {0};

    mlist_u32 list = mlist_init_u32(cfg);

    mlist_push_back_u32(list, 1);
    mlist_push_back_u32(list, 2);
    mlist_push_back_u32(list, 3);
    mlist_push_back_u32(list, 4);
    mlist_push_back_u32(list, 5);

    mlist_map_u32(list, u32_print);
    printf("---\n");

    mlist_free_u32(list);

    return 0;
}

