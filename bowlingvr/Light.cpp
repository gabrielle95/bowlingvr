#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Light.h"

Light::Light(Shader *shader,
	unsigned int lightIndex,
	glm::vec4 position,
	glm::vec4 ambient,
	glm::vec4 diffuse,
	glm::vec4 specular,
	bool isSpot
)
{
	this->shader = shader;
	this->lightIndex = lightIndex;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->isSpot = isSpot;
}

//each frame
void Light::Render()
{
	//position = mvMat * position;
	std::string idx = std::to_string(lightIndex);
	glUniform1i(shader->getUniLocation("Lights[" + idx + "].isEnabled"), 1);
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].ambient"), 1, glm::value_ptr(ambient));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].diffuse"), 1, glm::value_ptr(diffuse));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].specular"), 1, glm::value_ptr(specular));
	glUniform3fv(shader->getUniLocation("Lights[" + idx + "].position"), 1, glm::value_ptr(position));
}
