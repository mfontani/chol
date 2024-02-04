#include "tap.h"
unsigned int tap_test_number = 0;
void tap_done_testing(void)
{
    printf("1..%u\n", tap_test_number);
}
int tap_is_int_(int got, int wanted, char *test_description)
{
    tap_test_number++;
    if (got == wanted)
        printf("ok %u %s\n", tap_test_number, test_description);
    else
        printf("not ok %u %s (got %d wanted %d)\n",
            tap_test_number, test_description, got, wanted);
    return got == wanted;
}
int tap_is_ulong_(unsigned long got, unsigned long wanted, char *test_description)
{
    tap_test_number++;
    if (got == wanted)
        printf("ok %u %s\n", tap_test_number, test_description);
    else
        printf("not ok %u %s (got %lu wanted %lu)\n",
            tap_test_number, test_description, got, wanted);
    return got == wanted;
}
int tap_is_voidp_(void *got, void *wanted, char *test_description)
{
    tap_test_number++;
    if (got == wanted)
        printf("ok %u %s\n", tap_test_number, test_description);
    else
        printf("not ok %u %s (got %p wanted %p)\n",
            tap_test_number, test_description, got, wanted);
    return got == wanted;
}
void tap_run_self_tests(void)
{
    tap_diag("Running TAP tests...");
    tap_is_int(1, 1, "1 is 1");
    tap_is_voidp(NULL, NULL, "NULL is NULL");
    char *foo = "foo";
    char *bar = foo;
    tap_is_voidp(foo, bar, "foo is bar");
}
