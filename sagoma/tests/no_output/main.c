#include "../../../tap/tap.h"
#include <unistd.h>

#define SAGOMA_DONT_PRINT
#include "../../sagoma.h"

void test_no_output(void)
{
    tap_diag("Running test for no output");
    {
        sagoma_this(
            usleep(100);
            tap_is_int(sagoma.start_tsc > 0, 1, "sagoma.start_tsc > 0");
            tap_is_int(sagoma.end_tsc, 0, "sagoma.end_tsc == 0 (as profile going on)");
            sagoma_this_block("This block should not be printed",
                usleep(100);
            );
        );
        tap_is_int(sagoma.end_tsc > sagoma.start_tsc, 1, "sagoma.end_tsc > sagoma.start_tsc");
    }
}
