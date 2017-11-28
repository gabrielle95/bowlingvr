#pragma once

#ifndef DRAWABLEOBJ_H
#define DRAWABLEOBJ_H

#include <vector>
#include <iterator>
#include <SDL2/SDL.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "Shader.h"

#define BT_BOX 0
#define BT_SPHERE 1
#define BT_PLANE 2

class DrawableObj
{
public:
	DrawableObj();
	void SetTranslation(float x, float y, float z);
	void SetRotation(float rotation, float x, float y, float z);

	glm::mat4 GetTranslation();
	glm::mat4 GetRotation();

	void setShader(Shader *shader);

	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 m);

	~DrawableObj();
	bool Draw();
private:
	Shader *shader;
	GLuint vao, vbo, ebo;
	GLuint textureID;
	SDL_Surface* texture;
	std::vector<GLuint>elements;
	std::vector<float>vertices;

	glm::mat4 translation;
	glm::mat4 rotation;
	
	glm::mat4 modelMatrix;
	GLint modelUniform;

	void UpdateModelMatrix();
protected:
	void loadVertices(float data[], size_t size);
	void loadVertices(std::vector<float>data);

	void loadTexture(std::string fileName);

	void loadElements(GLuint data[], size_t size);
	void loadElements(std::vector<unsigned int>data);

	void Bind_vao();
	void Unbind_vao();
};
#endif