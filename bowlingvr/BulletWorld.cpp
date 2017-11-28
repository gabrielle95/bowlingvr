#include <iostream>
#include "BulletWorld.h"


BulletWorld *BulletWorld::bwInstancePtr = nullptr;
btDynamicsWorld *BulletWorld::bwWorld;

btDynamicsWorld* BulletWorld::Instance() /* singleton */
{
	if (BulletWorld::bwInstancePtr == nullptr)
	{
		BulletWorld::bwInstancePtr = new BulletWorld();
		return BulletWorld::bwWorld;
	}
	return BulletWorld::bwWorld;
}

BulletWorld::BulletWorld()
{
	this->bwColConfig = new btDefaultCollisionConfiguration();
	this->bwDispatcher = new btCollisionDispatcher(bwColConfig);
	this->bwBPInterface = new btDbvtBroadphase();
	this->bwConstraintSolver = new btSequentialImpulseConstraintSolver();

	/* create the bullet world*/
	this->bwWorld = new btDiscreteDynamicsWorld(this->bwDispatcher,
		this->bwBPInterface,
		this->bwConstraintSolver,
		this->bwColConfig);

	this->bwWorld->setGravity(btVector3(0, -9.807, 0)); //earth gravity

	this->debugDraw = new BulletDebugDraw();
	this->debugDraw->setDebugMode(1);
	this->bwWorld->setDebugDrawer(this->debugDraw);
}

BulletWorld::~BulletWorld()
{
	delete this->bwWorld;
	delete this->debugDraw;
	delete this->bwConstraintSolver;
	delete this->bwBPInterface;
	delete this->bwDispatcher;
}