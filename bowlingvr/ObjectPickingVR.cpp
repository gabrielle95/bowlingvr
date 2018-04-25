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
	btVector3 end = origin + direction * 2.f/*0.25f*/;

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
				p2p->m_setting.m_impulseClamp = 30.f;
				p2p->m_setting.m_tau = 0.001f;
				std::cout << "BULLET: Added constraint " << (int)p2p << std::endl;
				pickData.body = body;
				pickData.pickPos = pickPos;
				pickData.savedActivationState = savedState;
				pickData.constraint = p2p;
				pickData.oldPickingDist = (pickPos - origin).length();
				
				/*body->setActivationState(DISABLE_DEACTIVATION);
				btVector3 velocity = direction;
				velocity.normalize();
				velocity *= 0.25;
				body->setLinearVelocity(velocity);*/
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
		dynamicWorld->removeConstraint(pickData.constraint);
		std::cout << "BULLET: Removed constraint " << (int)pickData.constraint << std::endl;
		delete pickData.constraint;
		pickData.constraint = nullptr;
		//pickData.body->forceActivationState(pickData.savedActivationState);
		//pickData.body->forceActivationState(WANTS_DEACTIVATION);
		btVector3 vel = pickData.body->getLinearVelocity();
		vel *= 2;
		pickData.body->setLinearVelocity(vel);
		pickData.body = nullptr;
	}
}
