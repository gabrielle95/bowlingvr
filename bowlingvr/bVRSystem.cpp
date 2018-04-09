#include "bVRSystem.h"

bVRSystem::bVRSystem()
{
	vr_pointer = VR_Init(&eError, VRApplication_Scene);
	if (eError != VRInitError_None)
	{
		vr_pointer = NULL;
	}

}

bool bVRSystem::bVRInitVRCompositor()
{
	EVRInitError eError = VRInitError_None;
	if (!VRCompositor())
	{
		std::cout << "OPENVR::Unable to initialize VRCompositor" << std::endl;
		return false;
	}
	return true;
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



