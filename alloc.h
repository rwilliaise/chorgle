
#ifndef alloc_h
#define alloc_h

#include <stddef.h>

// alloc function type as compromise for compat
typedef void *(*alloc_t)(void *ptr, size_t size);

#endif // alloc_h
