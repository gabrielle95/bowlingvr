#pragma once

#include <iostream>
#include "Shader.h"
/* i have no clue what im doing */

class PostProcessing
{
public:
	GLuint fbo_texture;
	GLuint fbo;

	PostProcessing(float w, float h);
	void Init();
	void BindFPFramebuffer();
	void UnbindFPFramebuffer();
	void Bind();
	void PrintError();
	~PostProcessing();
private:
	
	GLuint rboDepth;
	float w, h;
};

//has multiple render targets
class Hdr
{
public:
	Hdr(float w, float h);
	void Init();
	void BindFPFramebuffer();
	void UnbindFPFramebuffer();
	void Bind(GLuint texture);
	void ConfigureShaders(Shader * blur, Shader * bloom);
	~Hdr();

	GLuint fbo_textures[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

private:
	GLuint fbo;
	GLuint rboDepth;
	float w, h;
};

//Multisample anti aliasing
class MSAA
{
public:
	MSAA(float w, float h);
	void BlitToFBO(GLuint fbo);
	void BindFBO();
	void UnbindFBO();

private:
	GLuint msFBO;
	GLuint msTex;
	float w, h;
	GLuint samples = 8;
	GLuint rboDepth;

	void Init();
};

class GeneralFramebuffer
{
public:
	GeneralFramebuffer(float w, float h);
	void BindFBO();
	void UnbindFBO();
	void Bind();
private:
	GLuint fbo;
	GLuint fbo_texture;
	GLuint rboDepth;
};