#include <Windows.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

//loading from external files
Shader::Shader(string vxname, string fgname, string gsname)
{
	this->ready = false;
	this->vxname = vxname;
	this->fgname = fgname;
	this->gsname = gsname;

	if (this->Load() < 1)
	{
		cout << "Error loading shaders!" << endl;
		return;
	}
	if (this->Compile() < 1)
	{
		cout << "Error compiling shaders!" << endl;
		return;
	}
	if (this->Link() < 1)
	{
		cout << "Error linking shaders!" << endl;
		return;
	}
	this->ready = true;
}

// compiling when embedded in code
Shader::Shader(const char *pchShaderName, const char * pchVertexShader, const char * pchFragmentShader, bool in_code, const char * pchGeometryShader)
{
	this->ready = false;
	this->vxname = pchShaderName;
	this->fgname = pchShaderName;
	this->gsname = "null";

	this->vxSource = pchVertexShader;
	this->fgSource = pchFragmentShader;

	if (pchGeometryShader != NULL)
	{
		this->gsname = pchShaderName;
		this->gsSource = pchGeometryShader;
	}
	
	if (this->Compile() < 1)
	{
		cout << "Error compiling shaders!" << endl;
		return;
	}
	if (this->Link() < 1)
	{
		cout << "Error linking shaders!" << endl;
		return;
	}
	this->ready = true;
}

int Shader::Load()
{
	this->vxFile.open(this->vxname, ios::in);
	if (!this->vxFile)
		return -1;
	this->fgFile.open(this->fgname, ios::in);
	if (!this->fgFile)
		return -2;

	

	this->vxStream << vxFile.rdbuf();
	this->fgStream << fgFile.rdbuf();

	if (gsname != "null")
	{
		this->gsFile.open(this->gsname, ios::in);
		if (!this->gsFile)
			return -3;

		this->gsStream << gsFile.rdbuf();
		this->gsSource = this->gsStream.str();
	}

	this->vxSource = this->vxStream.str();
	this->fgSource = this->fgStream.str();
	return 1;
}

int Shader::Compile()
{
	const char* c_str_source = this->vxSource.c_str();
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShader, 1, &c_str_source, 0);
	GLint compiled;
	glCompileShader(this->vertexShader);
	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &compiled);
	if (compiled == false)
	{
		unsigned int maxLen;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, (GLint *)&maxLen);
		char * vxCompLog = (char *)malloc(maxLen);
		glGetShaderInfoLog(this->vertexShader, maxLen, (GLsizei*)&maxLen, vxCompLog);
		cout << "Vertex Shader compile error: " << endl << vxCompLog << endl << endl;
		free(vxCompLog);
		return -1;
	}

	c_str_source = this->fgSource.c_str();
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragmentShader, 1, &c_str_source, 0);
	glCompileShader(this->fragmentShader);
	glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (compiled == false)
	{
		unsigned int maxLen;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, (GLint *)&maxLen);
		char * fgCompLog = (char *)malloc(maxLen);
		glGetShaderInfoLog(this->fragmentShader, maxLen, (GLsizei*)&maxLen, fgCompLog);
		cout << "Fragment Shader compile error: " << endl << fgCompLog << endl << endl;
		free(fgCompLog);
		return -2;
	}

	if (gsname != "null")
	{
		c_str_source = this->gsSource.c_str();
		this->geomShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(this->geomShader, 1, &c_str_source, 0);
		glCompileShader(this->geomShader);
		glGetShaderiv(this->geomShader, GL_COMPILE_STATUS, &compiled);
		if (compiled == false)
		{
			unsigned int maxLen;
			glGetShaderiv(this->geomShader, GL_INFO_LOG_LENGTH, (GLint *)&maxLen);
			char * fgCompLog = (char *)malloc(maxLen);
			glGetShaderInfoLog(this->geomShader, maxLen, (GLsizei*)&maxLen, fgCompLog);
			cout << "Geometry Shader compile error: " << endl << fgCompLog << endl << endl;
			free(fgCompLog);
			return -2;
		}
	}
	
	return 1;
}

bool Shader::Use()
{
	if (this->ready)
	{
		glUseProgram(this->shaderProgram);
	}
	return this->ready;
}

GLint Shader::getAttr(string attr)
{
	return glGetAttribLocation(this->shaderProgram, attr.c_str());
}

int Shader::Link()
{
	this->shaderProgram = glCreateProgram();

	glAttachShader(this->shaderProgram, this->vertexShader);
	if (gsname != "null")
		glAttachShader(this->shaderProgram, this->geomShader);

	glAttachShader(this->shaderProgram, this->fragmentShader);
	
	glLinkProgram(this->shaderProgram);

	GLint linked;

	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linked);

	if (linked == false)
	{
		unsigned int maxLen;
		glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, (GLint *)&maxLen);
		char *programLog = (char *)malloc(maxLen);
		glGetProgramInfoLog(this->shaderProgram, maxLen, (GLsizei*)&maxLen, programLog);
		cout << vxname << ": Shader link error: " << endl << programLog << endl << endl;
		free(programLog);
		return -1;
	}
	return 1;
}

GLint Shader::getUniLocation(string name)
{
	return glGetUniformLocation(this->shaderProgram, name.c_str());
}

void Shader::setUniMatrix(GLint location, glm::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)); 
}

void Shader::setInt(string location, int value)
{
	glUniform1i(getUniLocation(location), value);
}

void Shader::setFloat(string location, float value)
{
	glUniform1f(getUniLocation(location), value);
}

void Shader::setVec3(string location, glm::vec3 value)
{
	glUniform3fv(getUniLocation(location), 1, glm::value_ptr(value));
}

std::string Shader::getName()
{
	return this->vxname;
}


