#version 150

in vec3 Color;
in vec2 uvs;

out vec4 outColor;

uniform bool hasTexture;
uniform sampler2D in_texture;

void main()
{
	vec3 tmpCol = Color;
	if(hasTexture == true)
	{
		tmpCol *= texture(in_texture, uvs).rgb;
	}	
	outColor = vec4(tmpCol, 1.0);
}