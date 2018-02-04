#version 150

in vec3 Color;
in vec2 uvs;

out vec4 outColor;

uniform bool hasTexture;
uniform sampler2D texture_diffuse;

void main()
{
	vec3 tmpCol = Color;
	if(hasTexture == true)
	{
		tmpCol *= texture(texture_diffuse, uvs).rgb;
	}	
	outColor = vec4(tmpCol, 1.0);
}