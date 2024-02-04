#ifndef TAP_H
#define TAP_H
#include <stdio.h>
#include <stdlib.h>
extern unsigned int tap_test_number;
extern void tap_done_testing(void);
#define TAP_STRINGIFY(x) #x
#define TAP_TOSTRING(x) TAP_STRINGIFY(x)
#define TAP_AT " [ at " __FILE__ ":" TAP_TOSTRING(__LINE__) " ]"
#define tap_diag(fmt, ...) printf("# " fmt TAP_AT "\n", ##__VA_ARGS__)
extern int tap_is_int_(int got, int wanted, char *test_description);
#define tap_is_int(got, wanted, test_description) \
    tap_is_int_(got, wanted, test_description TAP_AT)
extern int tap_is_ulong_(unsigned long got, unsigned long wanted, char *test_description);
#define tap_is_ulong(got, wanted, test_description) \
    tap_is_ulong_(got, wanted, test_description TAP_AT)
extern int tap_is_voidp_(void *got, void *wanted, char *test_description);
#define tap_is_voidp(got, wanted, test_description) \
    tap_is_voidp_(got, wanted, test_description TAP_AT)
extern void tap_run_self_tests(void);
#endif
