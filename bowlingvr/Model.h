#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Model
{
public:
	Model(Shader *shader, const std::string& path);
	~Model();
	void Render(glm::mat4 pm, glm::mat4 vm);
private:
	bool success;
	Shader *shader;
	std::vector<Mesh*> meshes;
	size_t numMeshes;
	glm::mat4 modelMatrix;
	glm::mat4 mvMatrix;
	GLuint uPMatrix;
	GLuint uMVMatrix;
	std::string directory;

	void LoadModel(const std::string& path);
	void processNode(aiNode *node, const aiScene* scene);

};