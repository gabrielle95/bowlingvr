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
	Shader(std::string vxname, std::string fgname);
	bool Use();
	GLint getAttr(std::string attr);
	GLint getUniLocation(std::string name);
	void setUniMatrix(GLint location, glm::mat4 matrix);
	GLuint getShaderProgram();

private:
	std::ifstream vxFile, fgFile;
	std::string vxname, fgname;
	std::stringstream vxStream, fgStream;
	std::string vxSource, fgSource;
	GLuint vertexShader, fragmentShader, shaderProgram;
	bool ready;

	int Load();
	int Compile();
	int Link();
};
#endif