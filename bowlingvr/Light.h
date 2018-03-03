#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

class Light
{
public:
	Light(Shader *shader,
		unsigned int lightIndex,
		glm::vec4 position = { -0.3f, 0.5f, -0.3f, 1.0f },
		glm::vec4 ambient = { 0.05f, 0.05f, 0.05f, 0.2f },
		glm::vec4 diffuse = { 1.0f, 1.0f, 1.0f, 1.f },
		glm::vec4 specular = { 1.0f, 1.0f, 1.0f, 1.0f },
		bool isSpot = false
	);

	void Render();

private:
	Shader * shader;
	unsigned int lightIndex;
	bool isSpot;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	float shininess;
	float strength;
	//
	glm::vec4 eye_direction; //view * pos
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
	//
	glm::vec4 cone_direction;
	float spot_cos_cutoff; //how wide the spot is as a cosine
	float spot_exponent; //light falloff in the spot
};