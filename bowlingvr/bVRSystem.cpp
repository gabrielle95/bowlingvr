#include "bVRSystem.h"

bVRSystem::bVRSystem()
{
	vr_pointer = VR_Init(&eError, VRApplication_Scene);
	if (eError != VRInitError_None)
	{
		vr_pointer = NULL;
	}

	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		std::cout << "OPENVR::Failed to initialize Render Models!" << std::endl;
		if (vr_pointer != NULL)
			vr::VR_Shutdown();
		vr_pointer = NULL;
	}

}

bool bVRSystem::bVRInitVRCompositor()
{
	EVRInitError eError = VRInitError_None;
	if (!VRCompositor())
	{
		std::cout << "OPENVR::Unable to initialize VRCompositor." << std::endl;
		return false;
	}
	return true;
}

std::string bVRSystem::GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

std::string bVRSystem::RetrieveDriverDisplayVersion()
{
	m_strDriver = GetTrackedDeviceString(vr_pointer, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	m_strDisplay = GetTrackedDeviceString(vr_pointer, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);
	return std::string(m_strDriver + " " + m_strDisplay);
}



IVRSystem * bVRSystem::getVRpointer()
{
	return this->vr_pointer;
}

EVRInitError bVRSystem::getbVRError()
{
	return this->eError;
}

void bVRSystem::bVR_Shutdown()
{
	VR_Shutdown();
	vr_pointer = NULL;
}

// VR utilities static class
vr_utils::vr_utils()
{
}

vr_utils::~vr_utils()
{
}

glm::mat4 vr_utils::FromSteamVRMatrix(const HmdMatrix34_t & matPose)
{
	return glm::mat4(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
}
