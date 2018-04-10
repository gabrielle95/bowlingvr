#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <openvr/openvr.h>

using namespace vr;

class bVRSystem_exception : public std::runtime_error
{
public:
	bVRSystem_exception(const std::string& error) : std::runtime_error(error) {}
};

class bVRSystem {
public: //METHODS
	bVRSystem();

	bool bVRInitVRCompositor();

	std::string GetTrackedDeviceString(vr::IVRSystem * pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError);
	std::string RetrieveDriverDisplayVersion();

	IVRSystem * getVRpointer();
	EVRInitError getbVRError();

	void bVR_Shutdown();

public: //VARIABLES
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];

private: //VARIABLES
	IVRSystem * vr_pointer = NULL;
	EVRInitError eError = VRInitError_None;
	vr::IVRRenderModels *m_pRenderModels = NULL;
	std::string m_strDriver = "Unknown Display";
	std::string m_strDisplay = "Unknown Driver";



};

static class vr_convert
{
public:
	vr_convert();
	~vr_convert();

	glm::mat4 FromSteamVRMatrix(const HmdMatrix34_t &matPose);
};
