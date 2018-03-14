#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(std::string vxname, std::string fgname, std::string gsname = std::string("null"));
	bool Use();
	GLint getAttr(std::string attr);
	GLint getUniLocation(std::string name);
	void setUniMatrix(GLint location, glm::mat4 matrix);
	void setInt(std::string location, int value);
	void setFloat(std::string location, float value);
	void setVec3(std::string location, glm::vec3 value);
	GLuint getShaderProgram();

private:
	std::ifstream vxFile, fgFile, gsFile;
	std::string vxname, fgname, gsname;
	std::stringstream vxStream, fgStream, gsStream;
	std::string vxSource, fgSource, gsSource;
	GLuint vertexShader, fragmentShader, geomShader, shaderProgram;
	bool ready;

	int Load();
	int Compile();
	int Link();
};
#endif