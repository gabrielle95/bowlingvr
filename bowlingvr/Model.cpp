#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <SOIL2/SOIL2.h>
//#include <il/il.h>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

Model::Model(Shader * shader, const std::string& path)
{
	this->shader = shader;
	LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path, 
		aiProcess_FlipUVs | 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ASSIMP::ERROR " << importer.GetErrorString() << std::endl;
		this->success = false;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('\\'));
	this->processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene.
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		this->meshes.push_back(new Mesh(mesh, scene, this->directory));
	}

	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

void Model::Render(glm::mat4 pm, glm::mat4 vm)
{
	this->shader->Use();
	this->uPMatrix = this->shader->getUniLocation("uPMatrix");
	this->uMVMatrix = this->shader->getUniLocation("uMVMatrix");
	this->mvMatrix = vm * this->modelMatrix;
	this->shader->setUniMatrix(this->uMVMatrix, this->mvMatrix);
	this->shader->setUniMatrix(this->uPMatrix, pm);


	glm::vec4 lightAmbient = { .0f, .0f, .0f, .0f };
	glm::vec4 lightDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 lightSpecular = { 5.0f, 5.0f, 5.0f, 5.0f };
	glm::vec4 lightPosition = { 100.f, 100.f, 100.f, 1.0f };
	glm::vec4 lightPositionV = vm * lightPosition;

	glUniform4fv(shader->getUniLocation("light_ambient"), 1, glm::value_ptr(lightAmbient));
	glUniform4fv(shader->getUniLocation("light_diffuse"), 1, glm::value_ptr(lightDiffuse));
	glUniform4fv(shader->getUniLocation("light_specular"), 1, glm::value_ptr(lightSpecular));
	glUniform3fv(shader->getUniLocation("light_position"), 1, glm::value_ptr(lightPositionV));

	for (int i = 0; i < this->meshes.size(); ++i) {
		this->meshes.at(i)->Render(this->shader);
	}
}

