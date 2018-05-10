#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "Mesh.h"

//ASSIMP MODEL

//! Constructs an OpenGL model from Assimp. 
/*!
Imports the model from Assimp and processes the meshes. Can also be created using already processed meshes.
*/
class Model
{
public:
	Model(Shader *shader, const std::string& path);
	Model(Shader *shader, std::vector<Mesh*> meshEntries);
	void Render(Shader * shader);
	~Model();

	btRigidBody *rigidBody;
	std::vector<Mesh*> meshes;
	glm::mat4 modelMatrix;

protected:
	bool success;
	Shader *shader;
	
	size_t numMeshes;
	
	GLuint mUniform;

	glm::mat4 mvMatrix;
	GLuint uPMatrix;
	GLuint uMVMatrix;
	std::string directory;

	void LoadModel(const std::string& path);
	void processNode(aiNode *node, const aiScene* scene);
	void UpdateModelPosition();

	btCollisionShape *collisionShape = nullptr;
	btDefaultMotionState *motionstate = nullptr;

};