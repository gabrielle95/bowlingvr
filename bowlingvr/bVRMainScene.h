#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <openvr/openvr.h>

class bVRMainScene
{
public:
	bVRMainScene(vr::IVRSystem * vr_pointer);
	std::string GetTrackedDeviceString(vr::IVRSystem * pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError);
	~bVRMainScene();

private:
	vr::IVRSystem * m_pHMD = NULL;
	vr::EVRInitError eError = vr::VRInitError_None;
	
	



};