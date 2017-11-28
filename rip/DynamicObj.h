#pragma once
#ifndef DYNAMICOBJ_H
#define DYNAMICOBJ_H
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

class DynamicObj //extend?
{
public:
	DynamicObj(DrawableObj *drawable, btScalar mass, int shape);
	//btRigidBody *createRigidBody(DrawableObj *drawable);
	void addBodyToWorld(btDynamicsWorld *dynamicWorld);
	void updateDrawable();
	~DynamicObj();
private:
	btRigidBody *rigidBody;
	btCollisionShape *collisionShape;
	btDefaultMotionState *motionstate;
};
#endif