#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#include "../../dyn_arr.h"
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE
