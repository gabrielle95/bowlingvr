#include <Windows.h>
#include <GL/glew.h>

#include "Room.h"

/*stolen online*/
GLfloat room_vert[] = {
	-10.0f,  10.f, -10.0f,		0.502f, 0.502f, 0.502f, // 0 Top-left-front
	10.0f,  10.f, -10.0f,		0.502f, 0.502f, 0.502f, // 1 Top-right-front
	10.0f, 0.f, -10.0f,		0, 0, 0,				// 2 Bottom-right-front
	-10.0f, 0.f, -10.0f,		0.502f, 0.502f, 0.502f,  // 3 Bottom-left-front

	-10.0f,  10.f, 10.0f,		0.502f, 0.502f, 0.502f, // 4 Top-left-back
	10.0f,  10.f, 10.0f,		0.502f, 0.502f, 0.502f, // 5 Top-right-back
	10.0f, 0.f, 10.0f,		0.502f, 0.502f, 0.502f, // 6 Bottom-right-back
	-10.0f, 0.f, 10.0f,		0.412f, 0.412f, 0.412f  // 7 Bottom-left-back*/
};


GLuint room_elem[] = {
0, 1, 2,
2, 3, 0,
2, 3, 7,
7, 6, 2,
4,5,1,
1,0,4,
4,5,6,
6,7,4,
4,0,3,
3,7,4,
1,5,6,
6,2,1
};
Room::Room(Shader *shader) : DrawableObj()
{
	DrawableObj::setShader(shader);
	this->Bind_vao();
	this->loadVertices(room_vert, sizeof(room_vert) /** sizeof(GLfloat)*/);
	this->loadElements(room_elem, sizeof(room_elem) /* sizeof(GLuint)*/);
	this->posAttr = shader->getAttr("position");
	this->colAttr = shader->getAttr("color");

	glEnableVertexAttribArray(this->posAttr);
	glVertexAttribPointer(this->posAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(this->colAttr);
	glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	this->Unbind_vao();
}

Room::~Room()
{
	glDisableVertexAttribArray(this->colAttr);
	glDisableVertexAttribArray(this->posAttr);
}