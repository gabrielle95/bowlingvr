#include <GL/glew.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

Mesh::Mesh(aiMesh * aiMesh, const aiScene * scene, std::string dir)
{
	elementCount = aiMesh->mNumFaces * 3;
	for (int i = 0; i < aiMesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position[0] = aiMesh->mVertices[i].x;
		vertex.Position[1] = aiMesh->mVertices[i].y;
		vertex.Position[2] = aiMesh->mVertices[i].z;
		if (aiMesh->mNormals != NULL) {
			vertex.Normal[0] = aiMesh->mNormals[i].x;
			vertex.Normal[1] = aiMesh->mNormals[i].y;
			vertex.Normal[2] = aiMesh->mNormals[i].z;
		}
		else {
			vertex.Normal[0] = 0.0;
			vertex.Normal[1] = 0.0;
			vertex.Normal[2] = 1.0;
		}

		if (aiMesh->mTextureCoords[0])
		{
			vertex.TexCoords[0] = aiMesh->mTextureCoords[0][i].x;
			vertex.TexCoords[1] = aiMesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);
	}

	for (int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial * material = scene->mMaterials[aiMesh->mMaterialIndex];
	aiColor4D _def_color = { 0.0, 0.0, 0.0, 0.0 };
	aiColor4D _color = { 0.0, 0.0, 0.0, 1.0 };

	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &_color);
	ambientColor[0] = _color.r;
	ambientColor[1] = _color.g;
	ambientColor[2] = _color.b;
	ambientColor[3] = _color.a;

	_color = _def_color;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &_color);
	diffuseColor[0] = _color.r;
	diffuseColor[1] = _color.g;
	diffuseColor[2] = _color.b;
	diffuseColor[3] = _color.a;

	_color = _def_color;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &_color);
	specularColor[0] = _color.r;
	specularColor[1] = _color.g;
	specularColor[2] = _color.b;
	specularColor[3] = _color.a;

	_color = _def_color;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &_color);
	emissiveColor[0] = _color.r;
	emissiveColor[1] = _color.g;
	emissiveColor[2] = _color.b;
	emissiveColor[3] = _color.a;

	float opacity;
	if (AI_SUCCESS == aiGetMaterialFloatArray(material, AI_MATKEY_OPACITY, &opacity, NULL)) {
		ambientColor[3] = opacity;
		diffuseColor[3] = opacity;
		specularColor[3] = opacity;
	}
	shininess = 0.0f;
	aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &(shininess), NULL);

	ambientCount = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
	diffuseCount = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
	specularCount = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
	
	this->directory = dir;
	if (aiMesh->HasTextureCoords(0))
	{
		loadMaterialTextures(material, aiTextureType_AMBIENT);
		loadMaterialTextures(material, aiTextureType_DIFFUSE);
		loadMaterialTextures(material, aiTextureType_SPECULAR);
	}

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::loadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
	Texture texture;
	aiString str;
	unsigned int texCount = aiGetMaterialTextureCount(mat, type);
	if (!texCount) {
		texture.id = 0;
		texture.type = aiTextureType_NONE;
		//texture.path = NULL;
	}
	else {
		aiGetMaterialTexture(mat, type, 0, &str);
		texture.type = type;
		texture.path = str.C_Str();
		texture.id = textureFromFile(str.C_Str(), this->directory);
	}
	textures.push_back(texture);
}

GLuint Mesh::textureFromFile(const char * path, std::string dir)
{
	std::string filename = std::string(path);
	filename = directory + '\\' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, n;

	SDL_Surface * image = IMG_Load(filename.c_str());
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (image->format->BytesPerPixel == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	}
	

	

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Parameters
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);*/
	SDL_FreeSurface(image);
	return textureID;
}

void Mesh::Render(Shader *shader)
{
	size_t i = 0;
	GLuint texIx = 0;
	for (i = 0; i < textures.size(); i++)
	{
		switch (textures[i].type)
		{
		case aiTextureType_AMBIENT:
			glUniform1i(shader->getUniLocation("texture_ambient1"), i);
			break;
		case aiTextureType_DIFFUSE:
			glUniform1i(shader->getUniLocation("texture_diffuse1"), i);
			break;
		case aiTextureType_SPECULAR:
			glUniform1i(shader->getUniLocation("texture_specular1"), i);
			break;
		default:
			continue;
		}
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		texIx++;
	}

	glUniform4fv(shader->getUniLocation("ambientColor"), 1, glm::value_ptr(ambientColor));
	glUniform4fv(shader->getUniLocation("diffuseColor"), 1, glm::value_ptr(diffuseColor));
	glUniform4fv(shader->getUniLocation("specularColor"), 1, glm::value_ptr(specularColor));
	glUniform4fv(shader->getUniLocation("emissiveColor"), 1, glm::value_ptr(emissiveColor));
	glUniform1f(shader->getUniLocation("shininess"), shininess);
	glUniform1i(shader->getUniLocation("ambientTexCount"), ambientCount);
	glUniform1i(shader->getUniLocation("diffuseTexCount"), diffuseCount);
	glUniform1i(shader->getUniLocation("specularTexCount"), specularCount);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
