#include <stddef.h>
struct custom_foo {
    int a;
    int b;
};
struct custom_foo *custom_foo_new(int a, int b);
void custom_foo_free(struct custom_foo *f);
size_t custom_foo_hash(size_t size, const char *key);
#define DYN_KVP_VALUE_TYPE struct custom_foo
#define DYN_KVP_TYPE_NAME cfoo_kvp
#define DYN_KVP_KEY_TYPE const char *
#define DYN_KVP_HASH_FUNCTION custom_foo_hash
#include "../../dyn_kvp.h"
#undef DYN_KVP_HASH_FUNCTION
#undef DYN_KVP_KEY_TYPE
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
