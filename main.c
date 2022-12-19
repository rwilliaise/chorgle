
#include "window.h"
#include "graphics.h"

#ifdef __EMSCRIPTEN__
#include <stdlib.h>
static alloc_t global_alloc = &realloc;
#else
// mimalloc
#include <mimalloc.h>
static alloc_t global_alloc = &mi_realloc;
#endif

int start() {
	win_State *W = win_createstate(global_alloc);

	win_loop(W);
	return 0;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WinMain(HINSTANCE inst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow) {
	return start();
}

#else // _WIN32

int main() {
	return start();
}

#endif // else
