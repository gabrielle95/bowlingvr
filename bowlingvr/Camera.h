#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(Shader * shader, float w, float h);
	void SetShader(Shader *shader);
	void SetTranslation(float x, float y, float z);
	void SetRotation(float rotation, float x, float y, float z);
	void Translate(float x, float y, float z);
	void Rotate(float rotation, float x, float y, float z);
	void Update();
private:
	glm::mat4 projMatrix;
	glm::mat4 viewMat;
	glm::mat4 translation;
	glm::mat4 rotation;

	Shader * shader;

	GLint vpUniform;

	void UpdateViewMatrix();
};