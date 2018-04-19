#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(Shader * shader, float w, float h);
	Camera(glm::mat4 p, glm::mat4 v);
	void SetShader(Shader *shader);
	void SetTranslation(float x, float y, float z);
	void SetRotation(float rotation, float x, float y, float z);
	void Translate(float x, float y, float z);
	void Rotate(float rotation, float x, float y, float z);
	void Update();

	glm::vec3 getPosition();
	glm::vec3 getEyeDir();

	glm::mat4 getTranslation();
	glm::mat4 getRotation();

	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void setViewMatrix(glm::mat4 m);
	glm::vec3 direction;

	glm::mat4 projectionEye;
	glm::mat4 poseEye;

private:
	// p * v * m
	glm::mat4 projMatrix;
	glm::mat4 viewMat;
	glm::mat4 modelMat = glm::mat4(1.0); //identity




	glm::mat4 translation;
	glm::mat4 rotation;

	Shader * shader;


	GLint vUniform;
	GLint pUniform;

	void UpdateViewMatrix();
};
#endif