#pragma once

#include <string>
#include <openvr/openvr.h>

//creates openGL  model from VR models (controllers etc)
class bVRRenderModel
{
public:
	bVRRenderModel(const std::string & sRenderModelName);
	~bVRRenderModel();

	bool BInit(const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture);
	void Cleanup();
	void Draw();
	const std::string & GetName() const { return m_sModelName; }

private:
	GLuint m_glVertBuffer;
	GLuint m_glIndexBuffer;
	GLuint m_glVertArray;
	GLuint m_glTexture;
	GLsizei m_unVertexCount;
	std::string m_sModelName;

};