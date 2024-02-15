#include <stdio.h>

#include "../../sagoma.h"

int main(void)
{
    printf("Starting...\n");
    sagoma_init(s, "main.c");
    sagoma_block(s, foo);
    for (int i = 0; i < 10; i++)
    {
        sagoma_block(s, bar);
        for (int j = 0; j < 2; j++)
        {
            sagoma_block(s, baz);
            printf("Hello, world! i=%d, j=%d\n", i, j);
            sagoma_block_done(s);
        }
        sagoma_block_done(s);
    }
    sagoma_block_done(s);
    sagoma_print(s, 0, -1);
    printf("Done...\n");
}
