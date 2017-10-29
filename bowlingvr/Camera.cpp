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
	this->projMatrix = glm::perspective(glm::radians(60.0f), w/h, 0.05f, 1000.0f);
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
	this->shader->setUniMatrix(this->vpUniform, (this->projMatrix * this->viewMat));
}

void Camera::UpdateViewMatrix()
{
	this->viewMat = this->rotation * this->translation;
}