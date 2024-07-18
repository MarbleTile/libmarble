
#include <stdio.h>

#include <marble/types.h>

#define MHASH_TYPE u32
#include <marble/hash_typed.h>

void u32_print(u32 in) {
    printf("%u\n", in);
}

void mhash_print_u32(mhash_u32 table) {
    mhash_map_u32(table, u32_print);
    printf("---\n");
}

int main(void) {

    mhash_cfg_u32 cfg = {
        .copy = NULL,
        .free = NULL,
        .key_size = 32,
        .init_table_size = 64
    };

    mhash_u32 table = mhash_init_u32(cfg);

    mhash_put_u32(table, "test0", 0);
    mhash_put_u32(table, "test1", 1);
    mhash_put_u32(table, "test2", 2);
    mhash_put_u32(table, "test3", 3);

    mhash_print_u32(table);

    mhash_free_u32(table);

    return 0;
}

