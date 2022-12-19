
#ifndef graphics_h
#define graphics_h

#include "alloc.h"

typedef struct gfx_State gfx_State;

gfx_State *gfx_createstate(alloc_t alloc);
void gfx_free(gfx_State *G);

#endif // graphics_h
