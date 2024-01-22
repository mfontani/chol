#include "../../tap/tap.h"
extern void test_basic_foo(void);
extern void test_charp_foo(void);
int main(void) {
    tap_run_self_tests();
    test_basic_foo();
    test_charp_foo();
    tap_done_testing();
}
