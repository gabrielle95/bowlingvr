// Vertex shader for shadow map generation
#version 450 core
layout (location = 0) in vec3 position;

uniform mat4 model;

void main(void)
{
	gl_Position = model * vec4(position, 1.0);
}