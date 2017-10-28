#include <Windows.h>
#include <GL/glew.h>

#include "DrawableObj.h"

DrawableObj::DrawableObj()
{
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;
	glGenVertexArrays(1, &this->vao);

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
	this->Bind_vao();
	glDrawElements(GL_TRIANGLES, this->elements.size(), GL_UNSIGNED_INT, 0);
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
void DrawableObj::loadElements(std::vector<float>data)
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
}
