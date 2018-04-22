#pragma once

#include <openvr/openvr.h>

class ObjectPickingVR
{
public:
	ObjectPickingVR(vr::IVRSystem *pHmd);
	void ProcessButtonEvent(vr::VREvent_t event);

private:
	vr::IVRSystem * pHmd = NULL;
};