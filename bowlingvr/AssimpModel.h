#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "DrawableObj.h"
#include "Shader.h"

class AssimpModel : public DrawableObj
{
public:
	//bool LoadModelFromFile(Shader *shader, std::string filePath);

	static void FinalizeVBO();
	static void BindModelsVAO();

	void RenderModel();

	AssimpModel(Shader *shader, std::string filePath);
	AssimpModel(Shader *shader, std::vector<float> finalData, std::vector<unsigned int> elements);
	~AssimpModel();

	std::vector<unsigned int> elements;
	std::vector<float> finalData;
private:
	bool loaded;

};