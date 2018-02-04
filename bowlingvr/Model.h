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
#include <bullet/btBulletDynamicsCommon.h>
#include "Mesh.h"

static enum btBODIES {
	BALL, PLANE, PIN, COMPOSED, STATIC
};

class Model
{
public:
	Model(Shader *shader, const std::string& path);
	Model(Shader *shader, std::vector<Mesh*> meshEntries);
	~Model();
	void Render(glm::mat4 pm, glm::mat4 vm);
	bool InitPhysicsBody(btBODIES SHAPE,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btVector3 planeDimension = btVector3(0, 0, 0),
		btVector3 origin = btVector3(0, 0, 0)
		/* add friction and stuff later*/
	);

	btRigidBody *rigidBody;
	std::vector<Mesh*> meshes;
private:
	bool success;
	Shader *shader;
	
	size_t numMeshes;
	glm::mat4 modelMatrix;
	glm::mat4 mvMatrix;
	GLuint uPMatrix;
	GLuint uMVMatrix;
	std::string directory;

	void LoadModel(const std::string& path);
	void processNode(aiNode *node, const aiScene* scene);


	btCollisionShape *collisionShape = nullptr;
	btDefaultMotionState *motionstate = nullptr;

};