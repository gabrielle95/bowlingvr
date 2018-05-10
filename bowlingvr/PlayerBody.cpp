#include "BulletUtils.h"
#include "PlayerBody.h"

PlayerBody::PlayerBody(btVector3 origin)
{
	btTransform trans = BulletUtils::btTransFrom(this->modelMatrix);
	trans.setOrigin(origin);
	this->motionstate = new btDefaultMotionState(trans);
	btVector3 inertia = btVector3(0, 0, 0);

	this->collisionShape = new btCapsuleShape(btScalar(0.5f), btScalar(1.7f));
	btScalar mass = 62;

	this->collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo
	(mass, this->motionstate, this->collisionShape, inertia);
	rbInfo.m_friction = 20.0f;
	this->rigidBody = new btRigidBody(rbInfo);
	this->rigidBody->setAngularFactor(btVector3(0, 0, 0));
	this->rigidBody->forceActivationState(DISABLE_DEACTIVATION);
}
