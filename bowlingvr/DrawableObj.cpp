#include <Windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include "DrawableObj.h"

DrawableObj::DrawableObj()
{
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;
	this->textureID = 0;
	glGenVertexArrays(1, &this->vao);
}

void DrawableObj::setShader(Shader *shader)
{
	this->shader = shader;
	this->modelUniform = this->shader->getUniLocation("modelMatrix");
}

void DrawableObj::loadTexture(std::string fileName)
{
	this->texture = SDL_LoadBMP(fileName.c_str());
	if (this->texture)
	{
		glGenTextures(1, &this->textureID);
		if (this->textureID)
		{
			glBindTexture(GL_TEXTURE_2D, this->textureID);
			glTexImage2D(GL_TEXTURE_2D,
						 0, 
						 GL_RGB,
						 this->texture->w,
						 this->texture->h, 
						 0,
						 GL_BGR,
						 GL_UNSIGNED_BYTE,
						 this->texture->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void DrawableObj::Bind_vao()
{
	glBindVertexArray(this->vao);
}
void DrawableObj::Unbind_vao()
{
	glBindVertexArray(0);
}

bool DrawableObj::Draw()
{
	if (!this->vao || !this->vbo || !this->ebo)
	{
		return false;
	}
	this->shader->setUniMatrix(this->modelUniform, this->modelMatrix);
	this->Bind_vao();
	if(this->textureID)
		glBindTexture(GL_TEXTURE_2D, this->textureID);

	glDrawElements(GL_TRIANGLES, this->elements.size(), GL_UNSIGNED_INT, 0);

	if (this->textureID)
		glBindTexture(GL_TEXTURE_2D, 0);
	this->Unbind_vao();
	return true;
}

void DrawableObj::loadVertices(float data[], size_t size)
{
	this->vertices.insert(this->vertices.end(), data, data + size);
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER,  this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
	
}

void DrawableObj::loadVertices(std::vector<float>data)
{
	this->vertices.insert(this->vertices.end(), data.begin(), data.end());
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
}

void DrawableObj::loadElements(GLuint data[], size_t size)
{
	this->elements.insert(this->elements.end(), data, data + size);
	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->elements.size(), this->elements.data(), GL_STATIC_DRAW);
}
void DrawableObj::loadElements(std::vector<unsigned int>data)
{
	this->elements.insert(this->elements.end(), data.begin(), data.end());
	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->elements.size(), this->elements.data(), GL_STATIC_DRAW);
}

DrawableObj::~DrawableObj()
{
	if (this->ebo)
		glDeleteBuffers(1, &this->ebo);
	if (this->vbo)
		glDeleteBuffers(1, &this->vbo);
	glDeleteVertexArrays(1, &this->vao);

	if (this->textureID)
	{
		SDL_FreeSurface(this->texture);
	}
}

void DrawableObj::UpdateModelMatrix()
{
	this->modelMatrix = this->rotation * this->translation;
}

void DrawableObj::SetTranslation(float x, float y, float z)
{
	this->translation = glm::translate(glm::mat4(1.0f), glm::vec3(-x, -y, -z));
	this->UpdateModelMatrix();
}

void DrawableObj::SetRotation(float rotation, float x, float y, float z)
{
	this->rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(-x, -y, -z));
	this->UpdateModelMatrix();
}

glm::mat4 DrawableObj::getModelMatrix()
{
	return this->modelMatrix;
}

void DrawableObj::setModelMatrix(glm::mat4 m)
{
	this->modelMatrix = m;
}
