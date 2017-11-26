#include <Windows.h>
#include <GL/glew.h>

#include "TestShape.h"


GLfloat vert[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// Top-left 0
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// Top-right 1
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,// Bottom-right 2
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Bottom-left 3
	
};


GLuint elem[] = {
	0, 1, 2,
	2, 3, 0,

};
TestShape::TestShape(Shader *shader) : DrawableObj()
{
	this->Bind_vao();
	this->loadVertices(vert, sizeof(vert) * sizeof(GLfloat));
	this->loadElements(elem, sizeof(elem) * sizeof(GLint));
	this->posAttr = shader->getAttr("position");
	this->colAttr = shader->getAttr("color");
	this->uvAttr = shader->getAttr("uv");

	glEnableVertexAttribArray(this->posAttr);
	glVertexAttribPointer(this->posAttr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(this->colAttr);
	glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(this->uvAttr);
	glVertexAttribPointer(this->uvAttr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));

	this->loadTexture("test.bmp");
}

TestShape::~TestShape()
{

}