#include "../../../tap/tap.h"
#include <unistd.h> // for usleep

#define SAGOMA
#define SAGOMA_ASSERT
#define SAGOMA_TRACK_STATS
#define SAGOMA_CLOCK_CYCLES
#define SAGOMA_DEBUG
// #define SAGOMA_DONT_PRINT
#include "../../sagoma.h"

void test_ez(void)
{
    tap_diag("Running test for sagoma_this()");
    {
        sagoma_this(
            tap_is_int(sagoma.start_tsc > 0, 1, "sagoma.start_tsc > 0");
            tap_is_int(sagoma.block_count, 1, "sagoma.block_count == 1 (the main block)");
            tap_is_int(sagoma.stack_count, 1, "sagoma.stack_count == 1 (the main block)");
            tap_is_int(sagoma.blocks[0].stack_count, 1, "sagoma.block[0].stack_count == 1 (as profile going on)");
            tap_is_int(sagoma.end_tsc, 0, "sagoma.end_tsc == 0 (as profile going on)");
            usleep(100);
        );
        tap_is_int(sagoma.end_tsc > sagoma.start_tsc, 1, "sagoma.end_tsc > sagoma.start_tsc");
        tap_is_int(sagoma.block_count, 1, "sagoma.block_count == 1");
        tap_is_int(sagoma.stack_count, 0, "sagoma.stack_count == 0 (as profile completed / blocks popped)");
        tap_is_int(sagoma.blocks[0].entrance_count, 1, "sagoma.block[0].entrance_count == 1");
        tap_is_int(sagoma.blocks[1].entrance_count, 0, "sagoma.block[1].entrance_count == 0");
        tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
        tap_is_voidp(sagoma.stacks[1].block, NULL, "sagoma.stacks[1].block == NULL");
    }
    {
        sagoma_this(
            usleep(100);
            tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
            tap_is_voidp(sagoma.stacks[1].block, NULL, "sagoma.stacks[1].block == NULL");
            tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
            sagoma_this_block("second usleep",
                usleep(100);
                tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
                tap_is_voidp(sagoma.stacks[1].block, &sagoma.blocks[1], "sagoma.stacks[1].block == &sagoma.blocks[1]");
                tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
            );
            tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
            tap_is_voidp(sagoma.stacks[1].block, &sagoma.blocks[1], "sagoma.stacks[1].block == &sagoma.blocks[1]");
            tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
            sagoma_this_block("third usleep",
                usleep(100);
                tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
                tap_is_voidp(sagoma.stacks[1].block, &sagoma.blocks[2], "sagoma.stacks[1].block == &sagoma.blocks[2]");
                tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
            );
            tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
            tap_is_voidp(sagoma.stacks[1].block, &sagoma.blocks[2], "sagoma.stacks[1].block == &sagoma.blocks[2]");
            tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
        );
        tap_is_int(sagoma.end_tsc > sagoma.start_tsc, 1, "sagoma.end_tsc > sagoma.start_tsc");
        tap_is_int(sagoma.block_count, 3, "sagoma.block_count == 3");
        tap_is_int(sagoma.stack_count, 0, "sagoma.stack_count == 0 (as profile completed / blocks popped)");
        tap_is_int(sagoma.blocks[0].entrance_count, 1, "sagoma.block[0].entrance_count == 1");
        tap_is_int(sagoma.blocks[1].entrance_count, 1, "sagoma.block[1].entrance_count == 1");
        tap_is_int(sagoma.blocks[2].entrance_count, 1, "sagoma.block[2].entrance_count == 1");
        tap_is_int(sagoma.blocks[3].entrance_count, 0, "sagoma.block[3].entrance_count == 0");
        tap_is_voidp(sagoma.stacks[0].block, &sagoma.blocks[0], "sagoma.stacks[0].block == &sagoma.blocks[0]");
        tap_is_voidp(sagoma.stacks[1].block, &sagoma.blocks[2], "sagoma.stacks[1].block == &sagoma.blocks[2]");
        tap_is_voidp(sagoma.stacks[2].block, NULL, "sagoma.stacks[2].block == NULL");
    }
}
