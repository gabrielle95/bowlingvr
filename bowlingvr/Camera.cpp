#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Windows.h>
#include <GL/glew.h>

#include "Shader.h"
#include "Camera.h"


Camera::Camera(Shader *shader, float w, float h)
{
	this->shader = shader;
	//this->vUniform = this->shader->getUniLocation("mvpMatrix");
	this->vUniform = this->shader->getUniLocation("view");
	this->pUniform = this->shader->getUniLocation("projection");
	this->projMatrix = glm::perspective(glm::radians(45.0f), w/h, 0.05f, 100.f);
}

// for VR
Camera::Camera(glm::mat4 p, glm::mat4 v)
{
	this->projectionEye = p;
	this->poseEye = v;
}

void Camera::SetShader(Shader *shader)
{
	this->shader = shader;
	//this->vUniform = this->shader->getUniLocation("mvpMatrix");
	this->vUniform = this->shader->getUniLocation("view");
	this->pUniform = this->shader->getUniLocation("projection");
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
	//this->shader->setUniMatrix(this->vUniform, (this->projMatrix * this->viewMat/* * this->modelMat*/));
	this->shader->Use();
	this->shader->setUniMatrix(this->vUniform, this->viewMat);
	this->shader->setUniMatrix(this->pUniform, this->projMatrix);
	//glUniform3fv(this->shader->getUniLocation("viewPos"), 1, glm::value_ptr(getPosition()));
}

glm::vec3 Camera::getPosition()
{
	return glm::vec3(this->viewMat[3][0], this->viewMat[3][1], this->viewMat[3][2]);
}

glm::vec3 Camera::getEyeDir()
{
	return glm::vec3(-this->viewMat[2][0], -this->viewMat[2][1], -this->viewMat[2][2]);
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

void Camera::setViewMatrix(glm::mat4 m)
{
	this->viewMat = m;
}

void Camera::UpdateViewMatrix()
{
	//glm::vec3 up = glm::cross(this->direction, this->direction);
	//glm::vec3 lightPos(-2.0f, 2.5f, -25.0f);
	//this->viewMat = glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	this->viewMat = this->rotation * this->translation; //scale not included for now
}