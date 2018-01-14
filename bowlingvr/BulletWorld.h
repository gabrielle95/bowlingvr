#pragma once
#ifndef BULLETWORLD_H
#define BULLETWORLD_H
/* using bullet3-2.87*/
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletDebugDraw.h"
#include "CDebugDraw.h"

class BulletWorld
{
	
public:	
	BulletWorld();
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
#endif