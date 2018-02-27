#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <Windows.h>
#include <GL/glew.h>

#include "Shader.h"
#include "Camera.h"


Camera::Camera(Shader *shader, float w, float h)
{
	this->shader = shader;
	this->vpUniform = this->shader->getUniLocation("mvpMatrix");
	this->projMatrix = glm::perspective(glm::radians(45.0f), w/h, 0.05f, 1000.0f);
}

void Camera::SetShader(Shader *shader)
{
	this->shader = shader;
	this->vpUniform = this->shader->getUniLocation("mvpMatrix");
}

// 4,4,4 -> SetTranslation(0,0,0) = 0,0,0
void Camera::SetTranslation(float x, float y, float z)
{
	this->translation = glm::translate(glm::mat4(1.0f), glm::vec3(-x, -y, -z));
	this->UpdateViewMatrix();
}

// 4,4,4 -> Translate(2,0,0) = 6,4,4
void Camera::Translate(float x, float y, float z)
{
	this->translation = glm::translate(glm::mat4(this->translation), glm::vec3(-x, -y, -z));
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float rotation, float x, float y, float z)
{
	this->rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(-x, -y, -z));
	this->UpdateViewMatrix();
}

void Camera::Rotate(float rotation, float x, float y, float z)
{
	this->rotation = glm::rotate(glm::mat4(this->rotation), glm::radians(rotation), glm::vec3(-x, -y, -z));
	this->UpdateViewMatrix();
}

void Camera::Update()
{
	this->shader->setUniMatrix(this->vpUniform, (this->projMatrix * this->viewMat * this->modelMat));
}

glm::mat4 Camera::getTranslation()
{
	return this->translation;
}

glm::mat4 Camera::getRotation()
{
	return this->rotation;
}

glm::mat4 Camera::getModelMatrix()
{
	return this->modelMat;
}

glm::mat4 Camera::getViewMatrix()
{
	return this->viewMat;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return this->projMatrix;
}

void Camera::setModelMatrix(glm::mat4 m)
{
	this->modelMat = m;
}

void Camera::setViewMatrix(glm::mat4 m)
{
	this->viewMat = m;
}

void Camera::UpdateViewMatrix()
{
	//glm::vec3 up = glm::cross(this->direction, this->direction);
	//this->viewMat = glm::lookAt(this->position, this->position+this->direction, up);
	this->viewMat = this->rotation * this->translation; //scale not included for now
}