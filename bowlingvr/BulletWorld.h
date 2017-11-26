#pragma once
/* make it singleton */
/* using bullet3-2.87*/
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletDebugDraw.h"
#include"OpenGLMotionState.h"
//#include "BulletObject.h"

class BulletWorld
{
	
public:	
	BulletWorld();
	//static BulletWorld *Instance();	
	void bwUpdate(float step);
	btDynamicsWorld* BulletWorld::getDynamicsWorld();

	void HelloWorld();
	void CleanWorld();
	~BulletWorld();


	//BulletObject *objectManager;
	BulletDebugDraw *debugDraw;
	btDynamicsWorld *bwWorld;

private:
	//static BulletWorld *bwInstancePtr;
	btDispatcher *bwDispatcher;
	btBroadphaseInterface *bwBPInterface;
	btConstraintSolver *bwConstraintSolver;
	btCollisionConfiguration *bwColConfig;

	//std::vector<btRigidBody*>bodies;
	//btAlignedObjectArray<btCollisionShape*> collisionShapes;

};