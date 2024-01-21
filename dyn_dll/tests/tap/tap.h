#ifndef TAP_H
#define TAP_H
#include <stdio.h>
#include <stdlib.h>
extern unsigned int test_number;
extern void done_testing(void);
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT " [ at " __FILE__ ":" TOSTRING(__LINE__) " ]"
#define diag(fmt, ...) printf("# " fmt AT "\n", ##__VA_ARGS__)
extern void test_is_int_(int got, int wanted, char *test_description);
#define test_is_int(got, wanted, test_description) \
    test_is_int_(got, wanted, test_description AT)
extern void test_is_voidp_(void *got, void *wanted, char *test_description);
#define test_is_voidp(got, wanted, test_description) \
    test_is_voidp_(got, wanted, test_description AT)
extern void run_tap_tests(void);
#endif
