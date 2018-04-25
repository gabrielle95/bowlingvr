#pragma once

#include <iostream>
#include <openvr/openvr.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

struct PickedBodyData
{
	uint32_t controllerIndex;
	btRigidBody *body;
	btVector3 pickPos;
	int savedActivationState;
	btPoint2PointConstraint *constraint;
	btScalar oldPickingDist;
};

class ObjectPickingVR
{
public:

	ObjectPickingVR(vr::IVRSystem *pHmd, btDynamicsWorld *dynamicWorld);
	//void ProcessButtonEvent(vr::VREvent_t event);
	bool pickBody(btVector3 origin, btVector3 direction);
	bool movePickedBody(btVector3 origin, btVector3 direction);
	void removePickingConstraint();
	//bool performRayTest(btVector3 origin, btVector3 direction);

private:
	vr::IVRSystem * pHmd = NULL;
	btDynamicsWorld *dynamicWorld;

	PickedBodyData pickData;

};