#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
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
	SetTranslation(position);
}

void Light::SetTranslation(glm::vec3 pos)
{
	this->translation = glm::translate(glm::mat4(1.0f), pos);
	this->modelMatrix = this->translation;
}

//each frame
void Light::Render()
{
	//position = mvMat * position;
	std::string idx = std::to_string(lightIndex);
	glUniform1i(shader->getUniLocation("Lights[" + idx + "].isEnabled"), 1);
	
	/*if (this->isSpot)
	{
		glUniform1i(shader->getUniLocation("Lights[" + idx + "].isSpot"), 1);
		glUniform1f(shader->getUniLocation("Lights[" + idx + "].spotCosCutoff"), spot_cos_cutoff);
		glUniform1f(shader->getUniLocation("Lights[" + idx + "].spotExponent"), spot_exponent);
		glUniform4fv(shader->getUniLocation("Lights[" + idx + "].coneDirection"), 1, glm::value_ptr(cone_direction));
	}
	else
	{
		glUniform1i(shader->getUniLocation("Lights[" + idx + "].isSpot"), 0);
	}*/
	

	//glUniform1f(shader->getUniLocation("Lights[" + idx + "].constantAttenuation"), constant_attenuation);
	//glUniform1f(shader->getUniLocation("Lights[" + idx + "].linearAttenuation"), linear_attenuation);
	//glUniform1f(shader->getUniLocation("Lights[" + idx + "].quadraticAttenuation"), quadratic_attenuation);
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].ambient"), 1, glm::value_ptr(ambient));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].diffuse"), 1, glm::value_ptr(diffuse));
	glUniform4fv(shader->getUniLocation("Lights[" + idx + "].specular"), 1, glm::value_ptr(specular));
	glUniform3fv(shader->getUniLocation("LightPosition_worldspace[" + idx + "]"), 1, glm::value_ptr(position));
	glUniform3fv(shader->getUniLocation("Lights[" + idx + "].positon"), 1, glm::value_ptr(position));
}
