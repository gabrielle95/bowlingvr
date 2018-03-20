#version 330 core

struct MaterialProperties {
	vec4 emission;
//	vec4 ambient;
//	vec4 diffuse;
//	vec4 specular;
//	float shininess;
};

uniform MaterialProperties Material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D glowMap;
uniform float resolution;
uniform vec2 direction;

out vec4 FragColor;


void main()
{
	//vec4 glowFactor;
	//float radius = 1.0f;
	//float blur = radius / resolution;
	
	//glowFactor = Material.emission * texture(glowMap, TexCoords);
	FragColor = Material.emission;
}