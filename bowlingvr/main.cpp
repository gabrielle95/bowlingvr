#include "MainScene.h"
#include "MainSceneVR.h"



int main(int argc, char* argv[])
{
	vr::EVRInitError eError;
	vr::IVRSystem *vr_pointer = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
	{ 
		vr_pointer = NULL;
		std::cout << "OPENVR::Unable to init VR runtime: "<< vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
	}
	Application *application = new Application(vr_pointer);
	vr_pointer == NULL ? SetState<MainScene>(application) : SetState<MainSceneVR>(application);
	while (application->Run());
	delete application;
	return 0;
}