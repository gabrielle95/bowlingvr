#include "Windows.h"
#include <GL/glew.h>
#include "ObjLoader.h"

ObjLoader::ObjLoader(Shader *shader, std::string fileName) : DrawableObj()
{
	std::ifstream file;
	std::stringstream dataStream;

	this->elementID = 0;

	file.open(fileName, std::ios::in);
	std::cout << "loading obj..." << std::endl;
	if (file)
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream stringStream(line.substr(1));
			switch (line[0])
			{
			case '#':
				break;
			case 'v':
				float x, y, z;
				stringStream >> x >> y >> z;
				this->processVertexData(line, x, y, z);
				break;//?
			case 'f':
				std::string triplet;
				while (stringStream >> triplet)
				{
					this->processFaceData(triplet);
				}
				break;
			}
		}
		this->Bind_vao();
		this->loadVertices(this->finalData);
		this->loadElements(this->elements);

		this->posAttr = shader->getAttr("position");
		this->colAttr = shader->getAttr("color");
		this->uvAttr = shader->getAttr("uv");

		glEnableVertexAttribArray(this->posAttr);
		glVertexAttribPointer(this->posAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(this->colAttr);
		glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(this->uvAttr);
		glVertexAttribPointer(this->uvAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	
		this->Unbind_vao();

		this->vertices.clear();
		this->uvs.clear();
		this->finalData.clear();
		this->elements.clear();
		this->elementCache.clear();
		dataStream.clear();
		file.close();
		std::cout << "obj loaded successfully!" << std::endl;
	}
	else
	{
		std::cout << "failed to load obj file..." << std::endl;
	}
}

void ObjLoader::processVertexData(std::string type, float x, float y, float z)
{
	switch (type[1])
	{
	case 't':
		this->uvs.push_back(x);
		this->uvs.push_back(y);
		break;
	case ' ':
		this->vertices.push_back(x);
		this->vertices.push_back(y);
		this->vertices.push_back(z);
		break;
	}
}

void ObjLoader::processFaceData(std::string triplet)
{
	float v, vt, vn;
	std::istringstream stringStream(triplet);
	std::string token;

	std::getline(stringStream, token, '/');
	v = std::stoul(token, 0, 10);

	std::getline(stringStream, token, '/');
	vt = std::stoul(token, 0, 10);

	std::getline(stringStream, token, '/');
	vn = std::stoul(token, 0, 10);

	v -= 1; vt -= 1; vn -= 1;

	std::string elementString = std::to_string(v) + std::to_string(vt) + std::to_string(vn);
	//search cache for string ->we want to reuse
	auto element = this->elementCache.find(elementString);
	if (element != this->elementCache.end())
	{
		this->elements.emplace_back(element->second);
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			this->finalData.emplace_back(this->vertices[v * 3 + i]);
		}
		this->finalData.emplace_back(this->elementID / 256.0f);
		this->finalData.emplace_back(1);
		this->finalData.emplace_back(1);
		for (int i = 0; i < 2; i++)
		{
			this->finalData.emplace_back(this->uvs[vt * 2 + i]);
		}
		this->elements.emplace_back(this->elementID);
		this->elementCache.emplace(elementString, elementID);
		this->elementID += 1;
	}
}
