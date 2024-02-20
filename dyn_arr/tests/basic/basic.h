#include <stddef.h>
struct basic_foo {
    int a;
    int b;
};
struct basic_foo *basic_foo_new(int a, int b);
void basic_foo_free(struct basic_foo *f);
#define DYN_ARR_VALUE_TYPE struct basic_foo
#define DYN_ARR_TYPE_NAME bfoo_arr
#include "../../dyn_arr.h"
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE
