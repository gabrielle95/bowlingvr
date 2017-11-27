#include "BulletWorld.h"
#include <iostream>

/*BulletWorld *BulletWorld::bwInstancePtr = nullptr;

BulletWorld* BulletWorld::Instance() /* singleton */
/*{
	if (BulletWorld::bwInstancePtr == nullptr)
	{
		BulletWorld::bwInstancePtr = new BulletWorld();
	}
	return BulletWorld::bwInstancePtr;
}*/

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

	//this->objectManager = new BulletObjectManager(this->bwWorld);
	this->debugDraw = new BulletDebugDraw();
	this->debugDraw->setDebugMode(1);
	this->bwWorld->setDebugDrawer(this->debugDraw);
}

void BulletWorld::bwUpdate(float step)
{
	this->bwWorld->stepSimulation(step);
}

btDynamicsWorld* BulletWorld::getDynamicsWorld()
{
	return this->bwWorld;
}
void BulletWorld::HelloWorld()
{
	int i;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		this->bwWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new OpenGLMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		this->bwWorld->addRigidBody(body);
	}

	/// Do some simulation

	///-----stepsimulation_start-----
	for (i = 0; i < 150; i++)
	{
		this->bwWorld->stepSimulation(1.f / 60.f, 10);

		//print positions of all objects
		for (int j = this->bwWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = this->bwWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}
}
void BulletWorld::CleanWorld()
{
}
BulletWorld::~BulletWorld()
{
	//delete this->objectManager;
	delete this->bwWorld;
	delete this->debugDraw;
	delete this->bwConstraintSolver;
	delete this->bwBPInterface;
	delete this->bwDispatcher;
}