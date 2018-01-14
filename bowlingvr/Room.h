#pragma once

#ifndef ROOM_H
#define ROOM_H

#include "DrawableObj.h"
#include "Shader.h"

class Room : public DrawableObj
{
public:
	Room(Shader *shader);
	~Room();
private:
	GLint posAttr, colAttr;
	Shader *shader;
};
#endif