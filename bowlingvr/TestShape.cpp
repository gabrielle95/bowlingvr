#include <Windows.h>
#include <GL/glew.h>

#include "TestShape.h"

/*stolen online*/
GLfloat vert[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
};


GLuint elem[] = {
	0, 1, 2,
	2, 3, 0
};
TestShape::TestShape(Shader *shader) : DrawableObj()
{
	this->Bind_vao();
	this->loadVertices(vert, sizeof(vert) * sizeof(GLfloat));
	this->loadElements(elem, sizeof(elem) * sizeof(GLint));
	this->posAttr = shader->getAttr("position");
	this->colAttr = shader->getAttr("color");

	glEnableVertexAttribArray(this->posAttr);
	glVertexAttribPointer(this->posAttr, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(this->colAttr);
	glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));

}

TestShape::~TestShape()
{

}