#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

//! Creates a compiled shader object. 
/*!
Loads, compiles and links a desired shader source suppliying it in const char* format. Allows to upload values to a shader.
@see ShaderStrings.h
*/
class Shader
{
public:
	//Shader(std::string vxname, std::string fgname, std::string gsname = std::string("null"));
	Shader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader, const char *pchGeometryShader = NULL);
	GLint getUniLocation(std::string name);
	void setUniMatrix(GLint location, glm::mat4 matrix);
	void setInt(std::string location, int value);
	void setFloat(std::string location, float value);
	void setVec3(std::string location, glm::vec3 value);
	std::string getName();
	bool Use();	

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