#include "ObjectPickingVR.h"

using namespace vr;
///todo

ObjectPickingVR::ObjectPickingVR(vr::IVRSystem * pHmd)
{
	this->pHmd = pHmd;
}

void ObjectPickingVR::ProcessButtonEvent(vr::VREvent_t event)
{
	switch (event.data.controller.button)
	{
	case k_EButton_Grip:
		switch (event.eventType)
		{
		case VREvent_ButtonPress:
			break;

		case VREvent_ButtonUnpress:
			break;
		}
		break;

	case k_EButton_SteamVR_Trigger:
		switch (event.eventType)
		{
		case VREvent_ButtonPress:
			break;

		case VREvent_ButtonUnpress:
			break;
		}
		break;

	case k_EButton_SteamVR_Touchpad:
		switch (event.eventType)
		{
		case VREvent_ButtonPress:
			break;

		case VREvent_ButtonUnpress:
			break;

		case VREvent_ButtonTouch:
			break;

		case VREvent_ButtonUntouch:
			break;
		}
		break;

	case k_EButton_ApplicationMenu:
		switch (event.eventType)
		{
		case VREvent_ButtonPress:
			break;

		case VREvent_ButtonUnpress:
			break;
		}
		break;
	}
}
