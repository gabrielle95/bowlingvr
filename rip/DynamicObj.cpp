#include "BulletUtils.h"
#include "DrawableObj.h"
#include "DynamicObj.h"

DynamicObj::DynamicObj(DrawableObj *drawable, btScalar mass, int shape)
{
	switch (shape)
	{
	case BT_BOX:
		break; //todo
	case BT_SPHERE:
		this->collisionShape = new btSphereShape(btScalar(1.f));
	case BT_PLANE:
		this->collisionShape = new btStaticPlaneShape(btVector3(0,1,0), btScalar(0));
	}

	glm::mat4 modelMat = drawable->getModelMatrix();
	btTransform trans = BulletUtils::btTransFrom(modelMat);
	this->motionstate =	new btDefaultMotionState(trans);

	btRigidBody::btRigidBodyConstructionInfo rbInfo
	(mass, motionstate, collisionShape, btVector3(0, 0, 0));

	this->rigidBody = new btRigidBody(rbInfo);
	this->rigidBody->setUserPointer(drawable);
}

void DynamicObj::addBodyToWorld(btDynamicsWorld * dynamicWorld)
{
	dynamicWorld->addRigidBody(this->rigidBody);
}

void DynamicObj::updateDrawable()
{
	DrawableObj *upd = (DrawableObj *)this->rigidBody->getUserPointer();
	btTransform trans;
	this->motionstate->getWorldTransform(trans);
	glm::mat4 transmat = BulletUtils::glmMat4From(trans);
	upd->setModelMatrix(transmat);
}

DynamicObj::~DynamicObj()
{
	delete this->collisionShape;
	delete this->motionstate;
	delete this->rigidBody;
}
