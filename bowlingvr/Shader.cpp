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

}