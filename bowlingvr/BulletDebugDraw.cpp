#pragma once
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