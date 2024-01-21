#include "tap.h"
unsigned int test_number = 0;
void done_testing(void)
{
    printf("1..%u\n", test_number);
}
void test_is_int_(int got, int wanted, char *test_description)
{
    test_number++;
    if (got == wanted)
    {
        printf("ok %u %s\n", test_number, test_description);
        return;
    }
    printf("not ok %u %s (got %d wanted %d)\n",
        test_number, test_description, got, wanted);
}
void test_is_voidp_(void *got, void *wanted, char *test_description)
{
    test_number++;
    if (got == wanted)
    {
        printf("ok %u %s\n", test_number, test_description);
        return;
    }
    printf("not ok %u %s (got %p wanted %p)\n",
        test_number, test_description, got, wanted);
}
void run_tap_tests(void)
{
    diag("Running TAP tests...");
    test_is_int(1, 1, "1 is 1");
    test_is_voidp(NULL, NULL, "NULL is NULL");
    char *foo = "foo";
    char *bar = foo;
    test_is_voidp(foo, bar, "foo is bar");
}
