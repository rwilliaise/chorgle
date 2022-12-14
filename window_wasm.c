
#include "window.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>

#include <stdio.h>

#include <GLES2/gl2.h>

struct win_State {

};

static void win_render(void *p) {
	win_State *W = (win_State *) p;
	glClearColor(1.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

win_State *win_newstate(alloc_t alloc) {
	win_State *state = alloc(NULL, sizeof(win_State));
	printf("Creating window!\n");

	EmscriptenWebGLContextAttributes attribs;
	attribs.majorVersion = 2;
	attribs.minorVersion = 0;
	
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE gl = emscripten_webgl_create_context("#canvas", &attribs);

	if (gl == 0) {
		alloc(state, 0);
		return NULL;
	}

	emscripten_webgl_make_context_current(gl);

	return state;
}

void win_loop(win_State *W) {
	emscripten_set_main_loop_arg(win_render, W, 60, 1);
}
