#include "Windows.h"
#include <GL/glew.h>
#include "BulletUtils.h"
#include "AssimpModel.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void AssimpModel::RenderModel()
{
	if (this->motionstate != nullptr)
	{
		btTransform trans;
		this->motionstate->getWorldTransform(trans);
		glm::mat4 transmat = BulletUtils::glmMat4From(trans);
		this->modelMatrix = transmat;
	}

	this->shader->setUniMatrix(this->modelUniform, this->modelMatrix);
	this->shader->Use();
	for (int i = 0; i < this->meshEntries.size(); ++i) {
		this->meshEntries.at(i)->render();
	}
}

bool AssimpModel::InitPhysicsBody(btBODIES SHAPE, btScalar mass, btScalar radius, btScalar planeDimension, btVector3 origin)
{
	btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
	trans.setOrigin(origin);
	this->motionstate = new btDefaultMotionState(trans);
	btVector3 inertia = btVector3(0, 0, 0);


	switch (SHAPE) {
	case BALL:
		this->collisionShape = new btSphereShape(radius);
		
		if (mass != 0.f)
		{
			this->collisionShape->calculateLocalInertia(mass, inertia);
		}

		{
			btRigidBody::btRigidBodyConstructionInfo rbInfo
			(mass, this->motionstate, this->collisionShape, inertia);
			rbInfo.m_friction = 0.5f;
			rbInfo.m_rollingFriction = 0.2f;
			rbInfo.m_spinningFriction = 0.2f;
			rbInfo.m_linearDamping = 0.2f;
			this->rigidBody = new btRigidBody(rbInfo);
		}
		break;

	case PLANE:
		this->collisionShape = new btBoxShape(btVector3(planeDimension, planeDimension, planeDimension));
		{
			btRigidBody::btRigidBodyConstructionInfo rbInfo
			(btScalar(0.f), this->motionstate, this->collisionShape, btVector3(0, 0, 0));

			rbInfo.m_friction = 0.5f;
			this->rigidBody = new btRigidBody(rbInfo);
			this->rigidBody->setGravity(btVector3(0, 0, 0));
		}
		break;
	default:
		break;
	}
	return true;
}

AssimpModel::AssimpModel(Shader * shader, std::string filePath)
{
	Assimp::Importer importer;
	std::cout << "Assimp: Loading " << filePath << " ..." << std::endl;
	const aiScene *scene = importer.ReadFile(filePath, 
							aiProcess_CalcTangentSpace |
							aiProcess_Triangulate |
							aiProcess_JoinIdenticalVertices |
							aiProcess_SortByPType);

	if (!scene)
	{
		std::cerr << "Assimp: ERROR. Couldn't load model " << filePath << "!" << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
		return;
	}
	std::cout << "Assimp: SUCCESS loading " << filePath << "!" << std::endl;

	this->shader = shader;
	this->modelUniform = this->shader->getUniLocation("modelMatrix");

	for(int i = 0; i < scene->mNumMeshes; i++)
	{
		this->meshEntries.push_back(new AssimpModel::MeshEntry(scene->mMeshes[i]));
	}
}

AssimpModel::AssimpModel(Shader * shader, std::vector<MeshEntry*> meshEntries)
{
	this->shader = shader;
	this->modelUniform = this->shader->getUniLocation("modelMatrix");
	this->meshEntries = meshEntries;
	/*for (int i = 0; i < meshEntries.size(); i++)
	{
		this->meshEntries.push_back(new AssimpModel::MeshEntry(meshEntries[i]));
	}*/
}

AssimpModel::~AssimpModel()
{
	for (int i = 0; i < this->meshEntries.size(); ++i) {
		delete this->meshEntries.at(i);
	}
	this->meshEntries.clear();
}

AssimpModel::MeshEntry::MeshEntry(aiMesh * mesh)
{
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &this->vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &this->vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] texCoords;
	}


	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &this->vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] normals;
	}


	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &this->vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

AssimpModel::MeshEntry::~MeshEntry()
{
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &this->vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &this->vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &this->vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &this->vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &this->vao);
}

void AssimpModel::MeshEntry::render()
{
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
