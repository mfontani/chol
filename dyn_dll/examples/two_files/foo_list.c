#include "inc.h"
#define DYN_DLL_VALUE_TYPE struct foo
#define DYN_DLL_TYPE_NAME foo_list
#define DYN_DLL_IMPLEMENTATION
#include "../../dyn_dll.h"
#undef DYN_DLL_IMPLEMENTATION
#undef DYN_DLL_TYPE_NAME
#undef DYN_DLL_VALUE_TYPE
