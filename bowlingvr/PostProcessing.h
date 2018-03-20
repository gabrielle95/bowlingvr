#pragma once

#include <iostream>
#include "Shader.h"
/* i have no clue what im doing */

class PostProcessing
{
public:
	PostProcessing(float w, float h);
	void BindFPFramebuffer();
	void UnbindFPFramebuffer();
	void Bind();
	void ConfigureShaders(Shader * blur, Shader * bloom);
	void PrintError();

	GLuint fbo_textures[2];

	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

private:
	GLuint fbo;
	
	GLuint rboDepth;

	float w, h;

	void Init();
};