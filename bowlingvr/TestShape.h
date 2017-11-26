#pragma once

#include "DrawableObj.h"
#include "Shader.h"

class TestShape : public DrawableObj
{
public:
	TestShape(Shader *shader);
	~TestShape();
private:
	GLint posAttr, colAttr, uvAttr;
};