#pragma once

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>

typedef struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
} Vertex;

typedef struct Texture {
	GLuint id;
	aiTextureType type;
	aiString path;
} Texture;

class Mesh
{
public:
	Mesh(aiMesh *aiMesh, const aiScene * scene, std::string dir);
	void setupMesh();
	void loadMaterialTextures(aiMaterial *mat, aiTextureType type);
	GLuint textureFromFile(const char * path, std::string dir);
	void Render(Shader *shader);
	void RenderWithNoTextures();
	void RenderEmission(Shader *shader);

	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	size_t numVertices;
	size_t numIndices;
	size_t numTextures;
	bool hasTextures;
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	glm::vec4 emissiveColor;
	float shininess;
	unsigned int ambientCount;
	unsigned int diffuseCount;
	unsigned int specularCount;
	unsigned int elementCount;
	std::string directory;

};
