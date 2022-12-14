
#include "window.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
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
	gladLoadGL(glfwGetProcAddress);

	glEnable(GL_MULTISAMPLE);

	return W;
}

void win_loop(win_State *W) {
	while (!glfwWindowShouldClose(W->window)) {
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			  glColor3f(1.0f, 0.0f, 0.0f);
			  glVertex2f(-0.5f, -0.5f);
			  glColor3f(0.0f, 1.0f, 0.0f);
			  glVertex2f( 0.5f, -0.5f);
			  glColor3f(0.0f, 0.0f, 1.0f);
			  glVertex2f( 0.f,  0.5f);
	   glEnd();

		glfwSwapBuffers(W->window);
		glfwPollEvents();
	}
}

void win_free(win_State *W) {
	glfwDestroyWindow(W->window);
	glfwTerminate();
	W->alloc(W, 0);
}
