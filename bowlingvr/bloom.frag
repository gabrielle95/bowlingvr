#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{         
	vec3 col = texture(bloomBlur, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}