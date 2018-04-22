#pragma once

#include <openvr/openvr.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class ObjectPickingVR
{
public:
	ObjectPickingVR(vr::IVRSystem *pHmd);
	void ProcessButtonEvent(vr::VREvent_t event);
	void pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);

private:
	vr::IVRSystem * pHmd = NULL;
};