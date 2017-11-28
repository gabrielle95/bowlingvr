#pragma once

#ifndef CAMERA_H
#define CAMERA_H

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

	glm::mat4 getTranslation();
	glm::mat4 getRotation();

	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	//glm::vec3 position = glm::vec3(0,2,2);
	glm::vec3 direction;

private:
	// p * v * m
	glm::mat4 projMatrix;
	glm::mat4 viewMat;
	glm::mat4 modelMat = glm::mat4(1.0); //identity


	glm::mat4 translation;
	glm::mat4 rotation;

	Shader * shader;


	GLint vpUniform;

	void UpdateViewMatrix();
};
#endif