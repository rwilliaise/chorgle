
#version 200 es
layout (location = 0) in vec3 pos;

out vec4 color;

void main() {
	gl_Position = vec4(pos, 1.0);
	color = vec4(pos, 1.0);
}

