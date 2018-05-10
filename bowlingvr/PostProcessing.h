#pragma once

#include <iostream>
#include "Shader.h"

//! Creates a floating point framebuffer for post processing with a 2D texture attachment.
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
//! Creates a floating point framebuffer for post processing with a multiple color attachments.
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
//! Creates a framebuffer with multisampled texture attachment.
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

//! Creates a framebuffer with RGB texture attachment, no floating points (UNSIGNED_BYTE).
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