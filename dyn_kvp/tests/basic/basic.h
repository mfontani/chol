#include <stddef.h>
struct basic_foo {
    int a;
    int b;
};
struct basic_foo *basic_foo_new(int a, int b);
void basic_foo_free(struct basic_foo *f);
#define DYN_KVP_VALUE_TYPE struct basic_foo
#define DYN_KVP_TYPE_NAME bfoo_kvp
#include "../../dyn_kvp.h"
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
