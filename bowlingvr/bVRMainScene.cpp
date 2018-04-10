#include <assert.h>
#include "bVRMainScene.h"

bVRMainScene::bVRMainScene(vr::IVRSystem * vr_pointer)
{
	this->m_pHMD = vr_pointer;
	assert(vr_pointer != NULL);
	
}

