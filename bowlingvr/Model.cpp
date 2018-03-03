#include <GL/glew.h>
#include <assimp/Importer.hpp>


#include <assimp/postprocess.h>
//#include <SOIL2/SOIL2.h>
//#include <il/il.h>
#include <glm/gtc/type_ptr.hpp>
#include "BulletUtils.h"
#include "Model.h"

Model::Model(Shader * shader, const std::string& path)
{
	this->shader = shader;
	LoadModel(path);
}

Model::Model(Shader * shader, std::vector<Mesh*> meshEntries)
{
	this->shader = shader;
	this->meshes = meshEntries;
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


void Model::Render()
{
	if (this->motionstate != nullptr)
	{
		btTransform trans;
		this->rigidBody->getMotionState()->getWorldTransform(trans);
		glm::mat4 transmat = BulletUtils::glmMat4From(trans);
		this->modelMatrix = transmat;
	}

	this->shader->Use();
	this->shader->setUniMatrix(this->shader->getUniLocation("model"), modelMatrix);

	/*this->uPMatrix = this->shader->getUniLocation("uPMatrix");
	this->uMVMatrix = this->shader->getUniLocation("uMVMatrix");
	this->mvMatrix = vm * this->modelMatrix;
	
	this->shader->setUniMatrix(this->uMVMatrix, this->mvMatrix);
	this->shader->setUniMatrix(this->uPMatrix, pm);*/

	//this->shader->setUniMatrix(mUniform, pm * vm * this->modelMatrix);
	/*glm::vec4 lightAmbient = { 0.05f, 0.05f, 0.05f, 0.2f};
	glm::vec4 lightDiffuse = { 1.0f, 1.0f, 1.0f, 1.f };
	glm::vec4 lightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 lightPosition = { -0.3f, 0.5f, -0.3f, 1.0f };
	glm::vec4 lightPositionV = vm * lightPosition; //dir?*/
	//glm::vec4 eyeDirection = { -vm[2][0], -vm[2][1], -vm[2][2], 1.0 };

	/*glUniform1i(shader->getUniLocation("Lights[0].isEnabled"), 1);
	glUniform4fv(shader->getUniLocation("Lights[0].ambient"), 1, glm::value_ptr(lightAmbient));
	glUniform4fv(shader->getUniLocation("Lights[0].diffuse"), 1, glm::value_ptr(lightDiffuse));
	glUniform4fv(shader->getUniLocation("Lights[0].specular"), 1, glm::value_ptr(lightSpecular));
	glUniform3fv(shader->getUniLocation("Lights[0].position"), 1, glm::value_ptr(lightPositionV));
	*/
	for (int i = 0; i < this->meshes.size(); ++i) {
		this->meshes.at(i)->Render(this->shader);
	}
}


