#include "ObjectPickingVR.h"

using namespace vr;
///todo

ObjectPickingVR::ObjectPickingVR(vr::IVRSystem * pHmd, btDynamicsWorld *dynamicWorld)
{
	this->pHmd = pHmd;
	this->dynamicWorld = dynamicWorld;
	pickData.body = nullptr;
	pickData.constraint = nullptr;
}

bool ObjectPickingVR::pickBody(btVector3 origin, btVector3 direction)
{
	btVector3 end = origin + direction * 1.5f/*0.25f*/;

	btCollisionWorld::ClosestRayResultCallback RayCallback(origin, end);

	dynamicWorld->rayTest(origin, end, RayCallback);

	if (RayCallback.hasHit())
	{
		btVector3 pickPos = RayCallback.m_hitPointWorld;
		bool bIsStatic = RayCallback.m_collisionObject->isStaticObject();
		if (!bIsStatic)
		{
			btRigidBody *body = (btRigidBody *)btRigidBody::upcast(RayCallback.m_collisionObject);
			if (body)
			{
				int savedState = body->getActivationState();

				body->setActivationState(DISABLE_DEACTIVATION);
				btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
				btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
				dynamicWorld->addConstraint(p2p, true);
				std::cout << "BULLET: Added constraint " << (int)p2p << std::endl;
				pickData.body = body;
				pickData.oldFriction = pickData.body->getFriction();
				pickData.body->setFriction(200.f);

				pickData.pickPos = pickPos;
				pickData.savedActivationState = savedState;
				pickData.constraint = p2p;
				pickData.oldPickingDist = (pickPos - origin).length();

			}

			return true;
		}
		return false;
	}
	return false;
}

bool ObjectPickingVR::movePickedBody(btVector3 origin, btVector3 direction)
{
	if (pickData.body && pickData.constraint)
	{
		btPoint2PointConstraint *pickCon = static_cast<btPoint2PointConstraint*>(pickData.constraint);

		if (pickCon)
		{
			btVector3 dir = direction - origin;
			dir.normalize();
			dir *= pickData.oldPickingDist;

			btVector3 newPivotB(origin + dir);
			pickCon->setPivotB(newPivotB);
		}
	}
	return false;
}

void ObjectPickingVR::removePickingConstraint()
{
	if (pickData.constraint != nullptr)
	{
		pickData.body->setFriction(pickData.oldFriction);
		dynamicWorld->removeConstraint(pickData.constraint);

		

		std::cout << "BULLET: Removed constraint " << (int)pickData.constraint << std::endl;
		delete pickData.constraint;
		pickData.constraint = nullptr;
		btVector3 vel = pickData.body->getLinearVelocity();
		vel *= 3;
		pickData.body->setLinearVelocity(vel);
		pickData.body = nullptr;
	}
}
