#pragma once

#ifndef TESTSHAPE_H
#define TESTSHAPE_H

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
#endif