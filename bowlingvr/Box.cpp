#include <GL/glew.h>
#include "Box.h"

Box::Box(Shader *shader, const btVector3 &halfSize) : DrawableObj()
{
	GLfloat halfWidth = halfSize.x();
	GLfloat halfHeight = halfSize.y();
	GLfloat halfDepth = halfSize.z();

	GLfloat vertices[] =
	{
		halfWidth, halfHeight, halfDepth, 1.f, 0.f, 1.f,
		-halfWidth, halfHeight, halfDepth, 1.f, 0.f, 1.f,
		halfWidth, -halfHeight, halfDepth, 1.f, 0.f, 1.f,
		-halfWidth, -halfHeight, halfDepth, 1.f, 0.f, 1.f,
		halfWidth, halfHeight, -halfDepth, 1.f, 0.f, 1.f,
		-halfWidth, halfHeight, -halfDepth, 1.f, 0.f, 1.f,
		halfWidth, -halfHeight, -halfDepth, 1.f, 0.f, 1.f,
		-halfWidth, -halfHeight, -halfDepth, 1.f, 0.f, 1.f
	};

	GLuint indices[36] =
	{ 0, 1, 2, 3, 2, 1, 4, 0, 6, 6, 0, 2, 5, 1, 4, 4, 1, 0, 7, 3, 1, 7, 1, 5, 5,
		4, 7, 7, 4, 6, 7, 2, 3, 7, 6, 2 };

	this->Bind_vao();
	this->loadVertices(vertices, sizeof(vertices) * sizeof(GLfloat));
	this->loadElements(indices, sizeof(indices) * sizeof(GLuint));
	this->posAttr = shader->getAttr("position");
	this->colAttr = shader->getAttr("color");

	glEnableVertexAttribArray(this->posAttr);
	glVertexAttribPointer(this->posAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(this->colAttr);
	glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
}