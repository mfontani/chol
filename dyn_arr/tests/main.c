#include "../../tap/tap.h"
extern void test_basic_foo(void);
extern void test_basic_foo_remove(void);
extern void test_basic_foo_add(void);
extern void test_basic_foo_iterate(void);
int main(void) {
    tap_run_self_tests();
    test_basic_foo();
    test_basic_foo_remove();
    test_basic_foo_add();
    test_basic_foo_iterate();
    tap_done_testing();
}
