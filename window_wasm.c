
#include "graphics.h"
#include "window.h"

#include <stdio.h>
#include <math.h>

#include <emscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>

#include <GLES2/gl2.h>

#define min(x, y) x > y ? y : x

struct win_State {
	alloc_t alloc;
	gfx_State *G;
};

static EM_BOOL win_sizecallback(int type, const EmscriptenUiEvent *event, void *ud) {
	int width = event->windowInnerWidth;
	int height = event->windowInnerHeight;

	int size = min(width, height);

	glViewport(0, 0, size, size);
	emscripten_set_canvas_element_size("#canvas", size, size);
	return EM_TRUE;
}

win_State *win_createstate(alloc_t alloc) {
	win_State *W = alloc(NULL, sizeof(win_State));

	EmscriptenWebGLContextAttributes attrib = {
		.majorVersion = 2,
		.minorVersion = 0
	};

	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE gl = emscripten_webgl_create_context("#canvas", &attrib);

	if (gl == 0) {
		printf("Failed to initialize WebGL.\n");
		alloc(W, 0);
		return NULL;
	}

	emscripten_webgl_make_context_current(gl);
	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, win_sizecallback);

	W->G = gfx_createstate(alloc);

	int width, height;
	emscripten_get_screen_size(&width, &height);

	int size = min(width, height);
	glViewport(0, 0, size, size);
	emscripten_set_canvas_element_size("#canvas", size, size);
	
	return W;
}

static void win_render(void *p) {
	win_State *W = p;
	glClear(GL_COLOR_BUFFER_BIT);
}

void win_loop(win_State *W) {
	emscripten_set_main_loop_arg(win_render, W, 0, 1);
}

void win_free(win_State *W) {
	W->alloc(W, 0);
}

