#include <Windows.h>
#include <GL/glew.h>

#include "Shader.h"

using namespace std;

Shader::Shader(string vxname, string fgname)
{
	this->ready = false;
	this->vxname = vxname;
	this->fgname = fgname;

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

int Shader::Link()
{
	this->shaderProgram = glCreateProgram();

	glAttachShader(this->shaderProgram, this->vertexShader);
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
		cout << "Shader link error: " << endl << programLog << endl << endl;
		free(programLog);
		return -1;
	}
	return 1;
}