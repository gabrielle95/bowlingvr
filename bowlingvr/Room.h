#pragma once

#include "DrawableObj.h"
#include "Shader.h"

class Room : public DrawableObj
{
public:
	Room(Shader *shader);
	~Room();
private:
	GLint posAttr, colAttr;
};