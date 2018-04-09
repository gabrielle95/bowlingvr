#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shadowmap.h"

Shadowmap::Shadowmap(float w, float h)
{
	this->w = w;
	this->h = h;
	Init();
}

void Shadowmap::Init()
{
	glGenFramebuffers(1, &depthFBO);
	
	// create depth cubemap texture

	glGenTextures(1, &depthMapTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTex);
	
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// attach depth texture as FBO's depth buffer
	

	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTex, 0);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {  //Check for FBO completeness
		std::cout << "SHADOWMAPS::Error! FrameBuffer is not complete" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, w, h);
	
}



// 0. create depth cubemap transformation matrices
void Shadowmap::CreateCubemapMatrices(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
	shadowProj = glm::perspective(glm::radians(90.0f), w / h, near_plane, far_plane);

	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

//render methods
//FBO pass
void Shadowmap::RenderToDepthmap(Shader * shader)
{
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	for (unsigned int i = 0; i < 6; ++i)
		shader->setUniMatrix(shader->getUniLocation("shadowMatrices[" + std::to_string(i) + "]"), shadowTransforms[i]);
	shader->setFloat("far_plane", far_plane);
	shader->setVec3("lightPos", lightPos);
	//renderScene(simpleDepthShader);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadowmap::UnbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//normal pass
void Shadowmap::BindDepthTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTex);
	//render...
}

Shadowmap::~Shadowmap()
{
	glDeleteTextures(1, &depthMapTex);
	glDeleteBuffers(1, &depthFBO);
}
