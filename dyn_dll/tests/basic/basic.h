#include <stddef.h>
struct basic_foo {
    int a;
    int b;
};
struct basic_foo *basic_foo_new(int a, int b);
void basic_foo_free(struct basic_foo *f);
#define DYN_DLL_VALUE_TYPE struct basic_foo
#define DYN_DLL_TYPE_NAME bfoo_list
#include "../../dyn_dll.h"
#undef DYN_DLL_TYPE_NAME
#undef DYN_DLL_VALUE_TYPE
