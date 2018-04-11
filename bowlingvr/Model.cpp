#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
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

void Model::UpdateModelPosition()
{
	if (this->motionstate != nullptr)
	{
		btTransform trans;
		this->rigidBody->getMotionState()->getWorldTransform(trans);
		glm::mat4 transmat = BulletUtils::glmMat4From(trans);
		this->modelMatrix = transmat;
	}
}

/* NASTY HACK, BUT WORKS */
void Model::Render(Shader * shader)
{
	
	UpdateModelPosition();
	shader->Use();
	shader->setUniMatrix(shader->getUniLocation("model"), modelMatrix);

	if (shader->getName().compare("Model") == 0)
	{
		for (int i = 0; i < this->meshes.size(); ++i) {
			this->meshes.at(i)->Render(shader);
		}
	}
	else
	{
		for (int i = 0; i < this->meshes.size(); ++i) {
			this->meshes.at(i)->RenderWithNoTextures();
		}
	}

}


