
#include "graphics.h"

#include <stdio.h>
#include <GLES2/gl2.h>

#include "res/sprite.frag.h"
#include "res/sprite.vert.h"

struct gfx_State {
	alloc_t alloc;
	GLuint sprite_program;
};

static void gfx_loadshaders(gfx_State *G) {
	G->sprite_program = glCreateProgram();

	GLuint vert, frag;
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert, 1, (const char * const *) &sprite_vert_data, &sprite_vert_size);
	glShaderSource(frag, 1, (const char * const *) &sprite_frag_data, &sprite_frag_size);
	glCompileShader(vert);
	glCompileShader(frag);

	int compiled[2];
	char log[512];
	glGetShaderiv(vert, GL_COMPILE_STATUS, compiled);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled[1]);

	if ((*(long long *) compiled) != GL_TRUE) { 
		if (compiled[0]) {
			glGetShaderInfoLog(vert, 512, NULL, log);
			printf("ERROR: vsh compile:\n%s\n", log);
		}
		if (compiled[1]) {
			glGetShaderInfoLog(frag, 512, NULL, log);
			printf("ERROR: fsh compile:\n%s\n", log);
		}
		return;
	}

	glAttachShader(G->sprite_program, vert);
	glAttachShader(G->sprite_program, frag);
	glLinkProgram(G->sprite_program);

	glGetProgramiv(G->sprite_program, GL_LINK_STATUS, compiled);
	if (compiled[0] != GL_TRUE) {
		glGetProgramInfoLog(G->sprite_program, 512, NULL, log);	
		printf("ERROR: program link:\n%s\n", log);
		return;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

gfx_State *gfx_newstate(alloc_t alloc) {
	gfx_State *G = alloc(NULL, sizeof(gfx_State)); 
	G->alloc = alloc;

	gfx_loadshaders(G);

	return G;
}

void gfx_free(gfx_State *G) {
	G->alloc(G, 0);
}

