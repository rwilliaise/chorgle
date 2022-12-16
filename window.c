
#include "window.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES2
#endif

#include <GLFW/glfw3.h>

struct win_State {
	alloc_t alloc;
	GLFWwindow *window;
};

static void win_sizecallback(GLFWwindow *win, int width, int height) {
	glViewport(0, 0, width, height);
}

win_State *win_newstate(alloc_t alloc) {
	if (glfwInit() != GLFW_TRUE) {
		return NULL;
	}

	win_State *W = alloc(NULL, sizeof(win_State));
	W->alloc = alloc;

	glfwWindowHint(GLFW_SAMPLES, 4);
	W->window = glfwCreateWindow(640, 480, "chorgle", NULL, NULL);

	glfwSetWindowSizeCallback(W->window, win_sizecallback);

	glfwMakeContextCurrent(W->window);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	return W;
}

static void win_render(void *p) {
	win_State *W = p;
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(W->window);
	glfwPollEvents();
}

void win_loop(win_State *W) {
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(win_render, W, 60, 1);
#else
	while (!glfwWindowShouldClose(W->window)) {
		win_render(W);
	}
#endif
}

void win_free(win_State *W) {
	glfwDestroyWindow(W->window);
	glfwTerminate();
	W->alloc(W, 0);
}
