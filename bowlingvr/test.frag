#version 150

in vec3 Color;
in vec2 uvs;

out vec4 outColor;

uniform sampler2D in_texture;

void main()
{
	vec3 tmpCol = Color;
	tmpCol += texture(in_texture, uvs).rgb;
	outColor = vec4(tmpCol, 1.0);
}