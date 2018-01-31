#pragma once

#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct VERTEX
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

/*struct TEXTURE
{
	GLuint ID;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector<VERTEX> vertices;
	vector<GLuint> indices;
	vector<TEXTURE> textures;

	Mesh(vector<VERTEX> vertices, vector<GLuint> indices, vector<TEXTURE> textures);
};*/
#endif