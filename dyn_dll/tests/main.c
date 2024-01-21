#include "../../tap/tap.h"
extern void test_basic_foo(void);
int main(void) {
    tap_run_self_tests();
    test_basic_foo();
    tap_done_testing();
}
