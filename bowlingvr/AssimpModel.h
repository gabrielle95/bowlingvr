#pragma once


#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <string>
#include <vector>
#include "DrawableObj.h"
#include "Shader.h"


class AssimpModel
{
public:
	//bool LoadModelFromFile(Shader *shader, std::string filePath);
	static enum btBODIES {
		BALL, PLANE, PIN, COMPOSED, STATIC
	};

	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;

		MeshEntry(aiMesh *mesh);
		~MeshEntry();

		void load(aiMesh *mesh);
		void render();
	};

	std::vector<MeshEntry*> meshEntries;


	void RenderModel();

	bool InitPhysicsBody(btBODIES SHAPE,
		btScalar mass = btScalar(0),
		btScalar radius = btScalar(0),
		btScalar planeDimension = btScalar(0),
		btVector3 origin = btVector3(0, 0, 0)
		/* add friction and stuff later*/
	);
	AssimpModel(Shader *shader, std::string filePath);
	AssimpModel(Shader *shader, std::vector<MeshEntry*> meshEntries);
	~AssimpModel();

	Shader *shader;
	std::vector<unsigned int> elements;
	std::vector<float> finalData;

	btRigidBody *rigidBody;
private:
	bool loaded;
	glm::mat4 modelMatrix;
	GLint modelUniform;
	btCollisionShape *collisionShape;
	btDefaultMotionState *motionstate;
	

};