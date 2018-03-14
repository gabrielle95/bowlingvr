#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Light.h"

Light::Light(Shader *shader,
	unsigned int lightIndex,
	glm::vec4 position,
	bool isSpot,
	glm::vec4 ambient,
	glm::vec4 diffuse,
	glm::vec4 specular,
	glm::vec4 cone_direction,
	float spot_cos_cutoff,
	float spot_exponent
)
{
	this->shader = shader;
	this->lightIndex = lightIndex;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->isSpot = isSpot;
	this->cone_direction = cone_direction;
	this->spot_cos_cutoff = spot_cos_cutoff;
	this->spot_exponent = spot_exponent;
}

//each frame
void Light::Render(Shader *shader)
{
	std::string idx = std::to_string(lightIndex);
	glUniform1i(shader->getUniLocation("Lights[" + idx + "].isEnabled"), 1);
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].ambient"), 1, glm::value_ptr(ambient));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].diffuse"), 1, glm::value_ptr(diffuse));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].specular"), 1, glm::value_ptr(specular));
	glUniform3fv(shader->getUniLocation("LightPosition_worldspace[" + idx + "]"), 1, glm::value_ptr(position));
	glUniform3fv(shader->getUniLocation("Lights[" + idx + "].position"), 1, glm::value_ptr(position));
}

