#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class Shader
{
public:
	Shader(std::string vxname, std::string fgname);
	bool Use();
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