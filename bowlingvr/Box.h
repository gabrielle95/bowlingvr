#pragma once

#include "DrawableObj.h"
#include "Shader.h"

class Box : public DrawableObj
{
public:
	Box(Shader *shader, const btVector3 &halfSize);
	~Box();
private:
	GLint posAttr, colAttr;
};