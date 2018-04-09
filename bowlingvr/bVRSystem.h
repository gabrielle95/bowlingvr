#pragma once

#include <iostream>
#include <openvr/openvr.h>

using namespace vr;

class bVRSystem_exception : public std::runtime_error
{
public:
	bVRSystem_exception(const std::string& error) : std::runtime_error(error) {}
};

class bVRSystem {
public:
	bVRSystem();

	bool bVRInitVRCompositor();

	IVRSystem * getVRpointer();
	EVRInitError getbVRError();

	void bVR_Shutdown();

private:
	IVRSystem * vr_pointer = NULL;
	EVRInitError eError = VRInitError_None;

};
