#pragma once

#ifndef BULLETUTILS_H
#define BULLETUTILS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>

class BulletUtils
{
public:
	BulletUtils(void);
	~BulletUtils(void);

	static btTransform btTransFrom(const btVector3 & origin, const btQuaternion & rot = btQuaternion(0, 0, 0));
	static btTransform btTransFrom(const glm::mat4 & m);
	static glm::mat4 glmMat4From(const btTransform & trans);
};
#endif