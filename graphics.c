
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <GLES2/gl2.h>
#else // __EMSCRIPTEN__ 
#include <glad/gl.h>
#endif // else

#include "graphics.h"

#include "res/sprite.frag.h"
#include "res/sprite.vert.h"

struct gfx_State {
	alloc_t alloc;
	GLuint spriteprogram;
};

struct gfx_shadersource {
	const GLenum type; const char *source; const int size;
};

typedef struct gfx_shadersource gfx_shadersources[];

const gfx_shadersources SPRITE_SHADER = {
	{GL_FRAGMENT_SHADER, sprite_frag_data, sprite_frag_size},
	{GL_VERTEX_SHADER, sprite_vert_data, sprite_vert_size}
};

static void gfx_compileshader(GLuint *program, int count, const gfx_shadersources src) {
	*program = glCreateProgram();

	int shaders[count];
	int flag;
	char log[512];
	GLuint shader;
	for (int i = 0; i < count; i++) {
		struct gfx_shadersource shadersrc = src[i];
		const char * const *p = &shadersrc.source;

		shader = glCreateShader(GL_VERTEX_SHADER);
		shaders[i] = shader;

		glShaderSource(shader, 1, p, &shadersrc.size);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);

		if (flag != GL_TRUE) { 
			glGetShaderInfoLog(shader, 512, NULL, log);
			printf("ERROR: failed compile:\n%s\n", log);
			return;
		}

		glAttachShader(*program, shader);
	}
	glLinkProgram(*program);

	glGetProgramiv(*program, GL_LINK_STATUS, &flag);
	if (flag != GL_TRUE) {
		glGetProgramInfoLog(*program, 512, NULL, log);	
		printf("ERROR: program link:\n%s\n", log);
		return;
	}

	for (int i = 0; i < count; i++) {
		glDeleteShader(shaders[i]);
	}
}

gfx_State *gfx_newstate(alloc_t alloc) {
	gfx_State *G = alloc(NULL, sizeof(gfx_State));

	G->alloc = alloc;
	gfx_compileshader(&G->spriteprogram, 2, SPRITE_SHADER);

	return G;
}

void gfx_free(gfx_State *G) {
	glDeleteProgram(G->spriteprogram);

	G->alloc(G, 0);
}

