#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#include "../../dyn_dllist.h"
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE
