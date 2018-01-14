#include <iostream>
#include "BulletWorld.h"


BulletWorld *BulletWorld::bwInstancePtr = nullptr;

BulletWorld* BulletWorld::Instance() /* singleton */
{
	if (BulletWorld::bwInstancePtr == nullptr)
	{
		BulletWorld::bwInstancePtr = new BulletWorld();
		//return BulletWorld::bwWorld;
	}
	return BulletWorld::bwInstancePtr;
}

BulletWorld::BulletWorld()
{
	this->bwColConfig = new btDefaultCollisionConfiguration();
	this->bwDispatcher = new btCollisionDispatcher(bwColConfig);
	this->bwBPInterface = new btDbvtBroadphase();
	this->bwConstraintSolver = new btSequentialImpulseConstraintSolver();

	/* create the bullet world*/
	this->dynamicWorld = new btDiscreteDynamicsWorld(this->bwDispatcher,
		this->bwBPInterface,
		this->bwConstraintSolver,
		this->bwColConfig);

	this->dynamicWorld->setGravity(btVector3(0, -9.807, 0)); //earth gravity

	//this->debugDraw = new BulletDebugDraw_DeprecatedOpenGL();
	this->debugDraw = new CDebugDraw();
	this->debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	this->dynamicWorld->setDebugDrawer(this->debugDraw);
}

BulletWorld::~BulletWorld()
{
	delete this->dynamicWorld;
	delete this->debugDraw;
	delete this->bwConstraintSolver;
	delete this->bwBPInterface;
	delete this->bwDispatcher;
}