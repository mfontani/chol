#include "inc.h"
#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#define DYN_DLLIST_IMPLEMENTATION
#include "../../dyn_dllist.h"
#undef DYN_DLLIST_IMPLEMENTATION
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE
