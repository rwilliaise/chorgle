
#ifndef window_h
#define window_h

#include "alloc.h"

typedef struct win_State win_State;

win_State *win_createstate (alloc_t alloc);
void win_loop(win_State *W);
void win_free(win_State *W);

#endif // window_h
