#include <GL/glew.h>
//#include <GL/GL.h>
#include "BulletDebugDraw.h"


void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glm::vec3 src(from.x(), from.y(), from.z());
	glm::vec3 dst(to.x(), to.y(), to.z());
	glm::vec3 col(color.x(), color.y(), color.z());

	LINE l(src, dst, col);
	lines.push_back(l);
}

void BulletDebugDraw::ToggleDebugFlag(int flag)
{
	// checks if a flag is set and enables/
	// disables it
	if (m_debugMode & flag)
		// flag is enabled, so disable it
		m_debugMode = m_debugMode & (~flag);
	else
		// flag is disabled, so enable it
		m_debugMode |= flag;
}

void BulletDebugDraw::SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix) {
	glUseProgram(0); // Use Fixed-function pipeline (no shaders)
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&pViewMatrix[0][0]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&pProjectionMatrix[0][0]);
}