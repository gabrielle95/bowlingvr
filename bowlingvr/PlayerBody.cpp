#include "BulletUtils.h"
#include "PlayerBody.h"

PlayerBody::PlayerBody(btVector3 origin)
{
	btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
	trans.setOrigin(origin);
	this->motionstate = new btDefaultMotionState(trans);
	btVector3 inertia = btVector3(0, 0, 0);

	this->collisionShape = new btCapsuleShape(btScalar(1.f), btScalar(1.7f));
	btScalar mass = 62;

	this->collisionShape->calculateLocalInertia(mass, inertia);


	btRigidBody::btRigidBodyConstructionInfo rbInfo
	(mass, this->motionstate, this->collisionShape, inertia);
	rbInfo.m_friction = 10.0f;
	//rbInfo.m_rollingFriction = 0.1f;
	//rbInfo.m_spinningFriction = 0.1f;
	this->rigidBody = new btRigidBody(rbInfo);
	this->rigidBody->setAngularFactor(btVector3(0, 0, 0));
}
