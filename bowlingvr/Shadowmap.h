#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

class Shadowmap
{
public:
	Shadowmap(float w, float h);
	void CreateCubemapMatrices(glm::vec3 lightPos);
	void RenderToDepthmap(Shader *shader);

	void UnbindFBO();

	void BindDepthTexture();

	std::vector<glm::mat4> shadowTransforms;

private:
	GLuint depthFBO;
	GLuint depthMapTex;

	float w, h;
	float near_plane = 0.1f;
	float far_plane = 200.f;
	glm::vec3 lightPos;
	glm::mat4 shadowProj;

	void Init();

	void PrintError() {
		GLenum err;
		for (;;) {
			err = glGetError();
			if (err == GL_NO_ERROR) break;
			printf("Error: %d - %s\n", err, glewGetErrorString(err));
		}
	}
};