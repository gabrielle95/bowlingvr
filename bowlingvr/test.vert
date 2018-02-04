#version 150

in vec3 position;
in vec3 color;
in vec2 uv;

out vec3 Color;
out vec2 uvs;

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

void main()
{
	Color = color;
	//uvs = vec2(uv.x, 1.0 - uv.y);
	uvs = uv;
	//gl_Position =  mvpMatrix * modelMatrix * vec4(position, 1.0);
	gl_Position =  mvpMatrix * vec4(position, 1.0);
}