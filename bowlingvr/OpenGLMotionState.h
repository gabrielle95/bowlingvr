#pragma once

#ifndef OPENGLMS_H
#define OPENGLMS_H

#include <bullet/btBulletCollisionCommon.h>

class OpenGLMotionState : public btDefaultMotionState
{
public:
	OpenGLMotionState(const btTransform &transform) :
		btDefaultMotionState(transform)
	{
	}

	void GetWorldTransform(btScalar* transform)
	{
		btTransform trans;
		getWorldTransform(trans);
		trans.getOpenGLMatrix(transform);
	}
};
#endif