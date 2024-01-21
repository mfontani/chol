#include "tap/tap.h"
extern void test_basic_foo(void);
int main(void) {
    run_tap_tests();
    test_basic_foo();
    done_testing();
}
