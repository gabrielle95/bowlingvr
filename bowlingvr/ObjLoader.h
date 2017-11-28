#pragma once

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <glm/glm.hpp>

#include "DrawableObj.h"
#include "Shader.h"

class ObjLoader : public DrawableObj
{
public:
	ObjLoader(Shader *shader, std::string fileName);
private:
	void processVertexData(std::string type, float x, float y, float z);
	void processFaceData(std::string triplet);

	GLint posAttr, uvAttr, colAttr;

	std::vector<float> vertices;
	std::vector<float> uvs;
	std::unordered_map<std::string, unsigned int> elementCache;
	std::vector<unsigned int> elements;
	std::vector<float> finalData;
	unsigned int elementID;
};

#endif