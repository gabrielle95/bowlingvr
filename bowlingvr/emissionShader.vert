#version 330 core

//attempt at emissive glow

layout(location = 0) in vec3 position; //position modelspace
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos; //worldspace
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	TexCoords = texCoords;
	Normal = normal;
}
