#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#include "../../dyn_kvp.h"
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
