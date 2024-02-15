#include "../../tap/tap.h"
extern void test_ez(void);
extern void test_no_output(void);
extern void test_build_info(void);
int main(void) {
    tap_run_self_tests();
    test_ez();
    test_no_output();
    test_build_info();
    tap_done_testing();
}
