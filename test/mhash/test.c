
#include <stdio.h>

#include <libmarble.h>
#include <marble/hash.h>

void u32_print(void* val) {
    printf("%u\n", *(u32*)val);
}

void mhash_print(mhash table) {
    mhash_map(table, u32_print);
    printf("---\n");
}

int main(void) {
    
    mhash_cfg cfg = {
        .copy = NULL,
        .free = NULL,
        .key_size = 32,
        .init_table_size = 128
    };

    mhash table = mhash_init(cfg);

    u32 a = 1;
    u32 b = 2;

    mhash_put(table, "hello", &a);
    mhash_put(table, "world", &b);

    mhash_print(table);

    mhash_del(table, "hello");

    mhash_print(table);

    mhash_free(table);

    return 0;
}

