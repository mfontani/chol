#include <stdio.h>

#define SAGOMA 1
#define SAGOMA_ASSERT 1
#define SAGOMA_CLOCK_CYCLES 1
#include "../../sagoma.h"

int main(void)
{
    printf("Starting...\n");
    sagoma_init(s, "main.c");
    sagoma_block(s, foo);
    for (int i = 0; i < 10; i++)
    {
        sagoma_block(s, bar);
        if (i % 2 == 0)
            for (int j = 0; j < 6; j++)
            {
                sagoma_block(s, baz2);
                printf("Hello, world[2]! i=%d, j=%d\n", i, j);
                sagoma_block_done(s);
            }
        if (i % 6 == 0)
            for (int j = 0; j < 9; j++)
            {
                sagoma_block(s, baz6);
                printf("Hello, world[6]! i=%d, j=%d\n", i, j);
                sagoma_block_done(s);
            }
        if (i % 3 == 0)
            for (int j = 0; j < 5; j++)
            {
                sagoma_block(s, baz3);
                printf("Hello, world[3]! i=%d, j=%d\n", i, j);
                if (j % 2 == 0)
                {
                    sagoma_block(s, baz3_mod2);
                    printf("Hello, world[3][mod2]! i=%d, j=%d\n", i, j);
                    sagoma_block_done(s);
                }
                sagoma_block_done(s);
            }
        if (i % 5 == 0)
            for (int j = 0; j < 5; j++)
            {
                sagoma_block(s, baz5);
                printf("Hello, world[5]! i=%d, j=%d\n", i, j);
                sagoma_block_done(s);
            }
        sagoma_block_done(s);
    }
    for (int i = 0; i < 5; i++)
    {
        sagoma_block(s, quux);
        printf("Hello, world! i=%d\n", i);
        sagoma_block_done(s);
    }
    sagoma_block_done(s);
    sagoma_print(s, 0, -1);
    sagoma_print_color(s, 0, -1);
    printf("Done...\n");

    // sagoma_init_loop(s, "pippo");
}
