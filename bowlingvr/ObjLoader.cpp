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
		DrawableObj::setShader(shader);
		this->Bind_vao();
		this->loadVertices(this->finalData);
		this->loadElements(this->elements);

		this->positionAttr = shader->getAttr("position");
		this->colorAttr = shader->getAttr("color");
		this->uvsAttr = shader->getAttr("uv");

		glEnableVertexAttribArray(this->positionAttr);
		glVertexAttribPointer(this->positionAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(this->colorAttr);
		glVertexAttribPointer(this->colorAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(this->uvsAttr);
		glVertexAttribPointer(this->uvsAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
		this->Unbind_vao();

		std::cout << "obj loaded successfully!" << std::endl;
	}
	else
	{
		std::cout << "failed to load obj file..." << std::endl;
	}
	dataStream.clear();
	file.close();
}

ObjLoader::ObjLoader(Shader * shader, std::vector<float> finalData, std::vector<unsigned int> elements) : DrawableObj()
{
	this->finalData = finalData;
	this->elements = elements;
	this->Bind_vao();
	this->loadVertices(this->finalData);
	this->loadElements(this->elements);

	this->positionAttr = shader->getAttr("position");
	this->colorAttr = shader->getAttr("color");
	this->uvsAttr = shader->getAttr("uv");

	glEnableVertexAttribArray(this->positionAttr);
	glVertexAttribPointer(this->positionAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(this->colorAttr);
	glVertexAttribPointer(this->colorAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(this->uvsAttr);
	glVertexAttribPointer(this->uvsAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	this->Unbind_vao();
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

ObjLoader::~ObjLoader()
{
	this->finalData.clear();
	this->elements.clear();
	this->Unbind_vao();

	this->vertices.clear();
	this->uvs.clear();

	this->elementCache.clear();

}
