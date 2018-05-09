#pragma once

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "CDebugDraw.h"

//! Initialises the physics. 
/*!
Creates the btDynamicsWorld, the Bullet physics world.
*/
class BulletWorld
{
	
public:	
	BulletWorld();

	/**
	* Singleton: Returns the BulletWorld instance with btDynamics world pointer.
	* @return BulletWorld
	*/
	static BulletWorld *Instance();
	~BulletWorld();

	CDebugDraw *debugDraw;
	btDynamicsWorld *dynamicWorld;

private:
	static BulletWorld *bwInstancePtr;
	btDispatcher *bwDispatcher;
	btBroadphaseInterface *bwBPInterface;
	btConstraintSolver *bwConstraintSolver;
	btCollisionConfiguration *bwColConfig;

};