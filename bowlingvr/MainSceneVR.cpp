#include <GL/glew.h>
#include "MainScene.h"
#include "MainSceneVR.h"
#include "ShaderStrings.h"

MainSceneVR::MainSceneVR(Application * application) : MainScene(application)
{
	this->application = application;
}

MainSceneVR::~MainSceneVR()
{
	delete VRcontrollerShader;
	delete VRrendermodelShader;
	delete VRcompanionwindowShader;
	//delete mainScenePtr;
	vr::VR_Shutdown();
}

bool MainSceneVR::Init()
{
	this->pHmd = application->getVRpointer();

	if (!InitializeComponents())
	{
		return false;
	}

	/*************/
	/**SHADERS***/
	/************/

	this->VRcontrollerShader = new Shader("controller", c_VRcontrollerShaderVert, c_VRcontrollerShaderFrag);
	assert(this->VRcontrollerShader != nullptr);
	m_vr_controllerlocation = VRcontrollerShader->getUniLocation("matrix");

	this->VRrendermodelShader = new Shader("rendermodel", c_VRrendermodelShaderVert, c_VRrendermodelShaderFrag);
	assert(this->VRrendermodelShader != nullptr);
	m_vr_rendermodellocation = VRrendermodelShader->getUniLocation("matrix");

	this->VRcompanionwindowShader = new Shader("companionwindow", c_VRcompanionwindowShaderVert, c_VRcompanionwindowShaderFrag);
	assert(this->VRcompanionwindowShader != nullptr);

	//init main scene
	MainScene::Init();
	SetupCameras();
	SetupStereoRenderTargets();
	SetupCompanionWindow();
	SetupRenderModels();

	return true;
}

// MAIN LOOP

bool MainSceneVR::Update() //RENDER FRAME
{
	/* TIME */
	this->deltaNow = SDL_GetTicks();
	this->deltaTime = (this->deltaNow - this->deltaThen) / 1000.0;

	/* POLL */
	PollVREvent();

	this->dynamicWorld->stepSimulation(this->deltaTime, 5);

	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*this->CubeDepthMap->RenderToDepthmap(depthShader);
	RenderObjects(depthShader);
	this->CubeDepthMap->UnbindFBO();*/
	//RenderStereoTargets();

	RenderControllerAxes();
	RenderStereoTargets();
	RenderCompanionWindow();

	vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

	UpdateHMDMatrixPose();

	this->deltaThen = this->deltaNow;
	return true;
}

bool MainSceneVR::Destroy()
{
	return true;
}

bool MainSceneVR::InitializeComponents()
{
	if (!InitRenderModels())
	{
		return false;
	}

	if (!InitVRCompositor())
	{
		return false;
	}
	return true;
}

bool MainSceneVR::InitVRCompositor()
{
	if (!vr::VRCompositor())
	{
		std::cout << "OPENVR::Unable to initialize VRCompositor." << std::endl;
		return false;
	}
	return true;
}

bool MainSceneVR::InitRenderModels()
{
	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		std::cout << "OPENVR::Failed to initialize Render Models!" << std::endl;
		vr::VR_Shutdown();
		return false;
	}
	return true;
}

bool MainSceneVR::InitEyeBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc)
{
	glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

	glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);

	glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

	glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

	glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/**** INIT EYE CAMERAS ****/
void MainSceneVR::SetupCameras()
{
	m_mat4ProjectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
	m_mat4ProjectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
	m_mat4eyePosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	m_mat4eyePosRight = GetHMDMatrixPoseEye(vr::Eye_Right);
}

bool MainSceneVR::SetupStereoRenderTargets()
{
	if (!pHmd)
		return false;

	pHmd->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

	InitEyeBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
	InitEyeBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);

	return true;
}

void MainSceneVR::SetupCompanionWindow()
{
	if (!pHmd)
		return;

	std::vector<VertexDataWindow> vVerts;

	// left eye verts
	vVerts.push_back(VertexDataWindow(Vector2(-1, -1), Vector2(0, 1)));
	vVerts.push_back(VertexDataWindow(Vector2(0, -1), Vector2(1, 1)));
	vVerts.push_back(VertexDataWindow(Vector2(-1, 1), Vector2(0, 0)));
	vVerts.push_back(VertexDataWindow(Vector2(0, 1), Vector2(1, 0)));

	// right eye verts
	vVerts.push_back(VertexDataWindow(Vector2(0, -1), Vector2(0, 1)));
	vVerts.push_back(VertexDataWindow(Vector2(1, -1), Vector2(1, 1)));
	vVerts.push_back(VertexDataWindow(Vector2(0, 1), Vector2(0, 0)));
	vVerts.push_back(VertexDataWindow(Vector2(1, 1), Vector2(1, 0)));

	GLushort vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6 };
	m_uiCompanionWindowIndexSize = _countof(vIndices);

	glGenVertexArrays(1, &m_unCompanionWindowVAO);
	glBindVertexArray(m_unCompanionWindowVAO);

	glGenBuffers(1, &m_glCompanionWindowIDVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glCompanionWindowIDVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataWindow), &vVerts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_glCompanionWindowIDIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glCompanionWindowIDIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiCompanionWindowIndexSize * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void *)offsetof(VertexDataWindow, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void *)offsetof(VertexDataWindow, texCoord));

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MainSceneVR::SetupRenderModels()
{
	memset(m_rTrackedDeviceToRenderModel, 0, sizeof(m_rTrackedDeviceToRenderModel));

	if (!pHmd)
		return;

	for (uint32_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
		if (!pHmd->IsTrackedDeviceConnected(unTrackedDevice))
			continue;

		SetupRenderModelForTrackedDevice(unTrackedDevice);
	}

}

void MainSceneVR::SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex)
{
	if (unTrackedDeviceIndex >= vr::k_unMaxTrackedDeviceCount)
		return;

	// try to find a model we've already set up
	std::string sRenderModelName = GetTrackedDeviceString(pHmd, unTrackedDeviceIndex, vr::Prop_RenderModelName_String);
	bVRRenderModel *pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());

	if (!pRenderModel)
	{
		std::string sTrackingSystemName = GetTrackedDeviceString(pHmd, unTrackedDeviceIndex, vr::Prop_TrackingSystemName_String);
		std::cout << "OPENVR::Unable to load render model for tracked device with name " << sTrackingSystemName << std::endl;
	}
	else
	{
		m_rTrackedDeviceToRenderModel[unTrackedDeviceIndex] = pRenderModel;
		m_rbShowTrackedDevice[unTrackedDeviceIndex] = true;
	}
}

bVRRenderModel *MainSceneVR::FindOrLoadRenderModel(const char *renderModelString)
{
	bVRRenderModel *renderModel = NULL;

	for (std::vector<bVRRenderModel*>::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++)
	{
		if (!stricmp((*i)->GetName().c_str(), renderModelString))
		{
			renderModel = *i;
			break;
		}
	}

	if (!renderModel)
	{
		vr::RenderModel_t *r_model;
		vr::EVRRenderModelError error;

		while (true)
		{
			error = vr::VRRenderModels()->LoadRenderModel_Async(renderModelString, &r_model);
			if (error != vr::VRRenderModelError_Loading)
				break;

			Sleep(1);
		}

		if (error != vr::VRRenderModelError_None)
		{
			std::cout << "OPENVR:: Unable to load render model " << renderModelString << ": " << vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error) << std::endl;
			return NULL;
		}

		vr::RenderModel_TextureMap_t *r_texture;
		while (true)
		{
			error = vr::VRRenderModels()->LoadTexture_Async(r_model->diffuseTextureId, &r_texture);
			if (error != vr::VRRenderModelError_Loading)
				break;

			Sleep(1);
		}

		if (error != vr::VRRenderModelError_None)
		{
			std::cout << "OPENVR:: Unable to load render texture " << renderModelString << ": " << vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error) << std::endl;
			return NULL;
		}

		renderModel = new bVRRenderModel(renderModelString);
		if (!renderModel->BInit(*r_model, *r_texture))
		{
			std::cout << "OPENVR::Unable to create OpenGL model from " << renderModelString << std::endl;
			renderModel = NULL;
		}
		else
		{
			m_vecRenderModels.push_back(renderModel);
		}
		vr::VRRenderModels()->FreeRenderModel(r_model);
		vr::VRRenderModels()->FreeTexture(r_texture);
	}
	return renderModel;
}

std::string MainSceneVR::GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError)
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

Matrix4 MainSceneVR::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!pHmd)
		return Matrix4();

	float m_fNearClip = 0.05f;
	float m_fFarClip = 100.f;
	vr::HmdMatrix44_t mat = pHmd->GetProjectionMatrix(nEye, m_fNearClip, m_fFarClip);

	return Matrix4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
	/*return Matrix4(
		mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
		mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
		mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3],
		mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]
	);*/
}

Matrix4 MainSceneVR::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!pHmd)
		return Matrix4();

	vr::HmdMatrix34_t matEyeRight = pHmd->GetEyeToHeadTransform(nEye);
	Matrix4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);
	/*Matrix4 matrixObj(
		mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
		mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
		mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3],
		1,0,0,0
	);*/
	return matrixObj.invert();
	//return matrixObj;
}

Matrix4 MainSceneVR::GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	Matrix4 matMVP;
	if (nEye == vr::Eye_Left)
	{
		matMVP = m_mat4ProjectionLeft * m_mat4eyePosLeft * m_mat4HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = m_mat4ProjectionRight * m_mat4eyePosRight *  m_mat4HMDPose;
	}

	return matMVP;
}
/*****************************RENDERING ***********************************************/
//call in loop
void MainSceneVR::PollVREvent()
{
	vr::VREvent_t event;
	if (pHmd->PollNextEvent(&event, sizeof(event)))
	{
		switch (event.eventType)
		{
		case vr::VREvent_TrackedDeviceActivated:
			//add rendering for device model
			SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
			std::cout << "OPENVR:: Device " << event.trackedDeviceIndex << " attached." << std::endl;
			break;
		case vr::VREvent_TrackedDeviceDeactivated:
			std::cout << "OPENVR:: Device " << event.trackedDeviceIndex << " detached." << std::endl;
			break;
		case vr::VREvent_TrackedDeviceUpdated:
			std::cout << "OPENVR:: Device " << event.trackedDeviceIndex << " updated." << std::endl;
			break;

			//and so on, can test for any amount of vr events
		default:
			break;
			/*default:
			printf("OPENVR::Event: %d\n", event.eventType);*/
		}
	}

	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
		vr::VRControllerState_t state;
		if (pHmd->GetControllerState(unDevice, &state, sizeof(state)))
		{
			m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
		}
	}
	//Output tracking data or do other things
}

void MainSceneVR::UpdateHMDMatrixPose()
{
	if (!pHmd)
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = glmFromSteamVRMatrix(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (pHmd->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		m_mat4HMDPose.invert();
	}
}

Matrix4 MainSceneVR::glmFromSteamVRMatrix(const vr::HmdMatrix34_t & matPose)
{
	Matrix4 pose(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	/*Matrix4 pose(
		matPose.m[0][0], matPose.m[0][1], matPose.m[0][2], matPose.m[0][3],
		matPose.m[1][0], matPose.m[1][1], matPose.m[1][2], matPose.m[1][3],
		matPose.m[2][0], matPose.m[2][1], matPose.m[2][2], matPose.m[2][3],
		1, 0, 0, 0
	);*/
	return pose;
}

void MainSceneVR::RenderControllerAxes()
{
	// Don't attempt to update controllers if input is not available
	if (!pHmd->IsInputAvailable())
		return;

	std::vector<float> vertdataarray;

	m_uiControllerVertcount = 0;
	m_iTrackedControllerCount = 0;

	for (vr::TrackedDeviceIndex_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; ++unTrackedDevice)
	{
		if (!pHmd->IsTrackedDeviceConnected(unTrackedDevice))
			continue;

		if (pHmd->GetTrackedDeviceClass(unTrackedDevice) != vr::TrackedDeviceClass_Controller)
			continue;

		m_iTrackedControllerCount += 1;

		if (!m_rTrackedDevicePose[unTrackedDevice].bPoseIsValid)
			continue;

		const Matrix4 & mat = m_rmat4DevicePose[unTrackedDevice];

		Vector4 center = mat * Vector4(0, 0, 0, 1);

		for (int i = 0; i < 3; ++i)
		{
			Vector3 color(0, 0, 0);
			Vector4 point(0, 0, 0, 1);
			point[i] += 0.05f;  // offset in X, Y, Z
			color[i] = 1.0;  // R, G, B
			point = mat * point;
			vertdataarray.push_back(center.x);
			vertdataarray.push_back(center.y);
			vertdataarray.push_back(center.z);

			vertdataarray.push_back(color.x);
			vertdataarray.push_back(color.y);
			vertdataarray.push_back(color.z);

			vertdataarray.push_back(point.x);
			vertdataarray.push_back(point.y);
			vertdataarray.push_back(point.z);

			vertdataarray.push_back(color.x);
			vertdataarray.push_back(color.y);
			vertdataarray.push_back(color.z);

			m_uiControllerVertcount += 2;
		}

		Vector4 start = mat * Vector4(0, 0, -0.02f, 1);
		Vector4 end = mat * Vector4(0, 0, -39.f, 1);
		Vector3 color(.92f, .92f, .71f);

		vertdataarray.push_back(start.x); vertdataarray.push_back(start.y); vertdataarray.push_back(start.z);
		vertdataarray.push_back(color.x); vertdataarray.push_back(color.y); vertdataarray.push_back(color.z);

		vertdataarray.push_back(end.x); vertdataarray.push_back(end.y); vertdataarray.push_back(end.z);
		vertdataarray.push_back(color.x); vertdataarray.push_back(color.y); vertdataarray.push_back(color.z);
		m_uiControllerVertcount += 2;
	}

	// Setup the VAO the first time through.
	if (m_unControllerVAO == 0)
	{
		glGenVertexArrays(1, &m_unControllerVAO);
		glBindVertexArray(m_unControllerVAO);

		glGenBuffers(1, &m_glControllerVertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);

		GLuint stride = 2 * 3 * sizeof(float);
		uintptr_t offset = 0;

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void *)offset);

		offset += sizeof(Vector3);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void *)offset);

		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);

	// set vertex data if we have some
	if (vertdataarray.size() > 0)
	{
		//$ TODO: Use glBufferSubData for this...
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertdataarray.size(), &vertdataarray[0], GL_STREAM_DRAW);
	}
}

void MainSceneVR::RenderScene(vr::Hmd_Eye nEye)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	bool bIsInputAvailable = pHmd->IsInputAvailable();
	if (bIsInputAvailable)
	{
		// draw the controller axis lines
		VRcontrollerShader->Use();
		glUniformMatrix4fv(VRcontrollerShader->getUniLocation("matrix"), 1, GL_FALSE, GetCurrentViewProjectionMatrix(nEye).get());
		//VRcontrollerShader->setUniMatrix(VRcontrollerShader->getUniLocation("matrix"), GetCurrentViewProjectionMatrix(nEye));
		glBindVertexArray(m_unControllerVAO);
		glDrawArrays(GL_LINES, 0, m_uiControllerVertcount);
		glBindVertexArray(0);
	}

	VRrendermodelShader->Use();

	for (uint32_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
		if (!m_rTrackedDeviceToRenderModel[unTrackedDevice] || !m_rbShowTrackedDevice[unTrackedDevice])
			continue;

		const vr::TrackedDevicePose_t & pose = m_rTrackedDevicePose[unTrackedDevice];
		if (!pose.bPoseIsValid)
			continue;

		if (!bIsInputAvailable && pHmd->GetTrackedDeviceClass(unTrackedDevice) == vr::TrackedDeviceClass_Controller)
			continue;

		const Matrix4 & matDeviceToTracking = m_rmat4DevicePose[unTrackedDevice];
		Matrix4 matMVP = GetCurrentViewProjectionMatrix(nEye) * matDeviceToTracking;
		glUniformMatrix4fv(VRrendermodelShader->getUniLocation("matrix"), 1, GL_FALSE, matMVP.get());
		//VRrendermodelShader->setUniMatrix(VRrendermodelShader->getUniLocation("matrix"), matMVP);

		m_rTrackedDeviceToRenderModel[unTrackedDevice]->Draw();

	}
	glUseProgram(0);
}

/* renders left eye and right eye to framebuffers */
void MainSceneVR::RenderStereoTargets()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_MULTISAMPLE);

	// Left Eye
	glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
	RenderScene(vr::Eye_Left);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);

	glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
	GL_COLOR_BUFFER_BIT,
	GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glEnable(GL_MULTISAMPLE);

	// Right Eye
	glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
	RenderScene(vr::Eye_Right);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);

	glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
	GL_COLOR_BUFFER_BIT,
	GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void MainSceneVR::RenderCompanionWindow()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, application->w(), application->h());

	glBindVertexArray(m_unCompanionWindowVAO);
	VRcompanionwindowShader->Use();

	// render left eye (first half of index array )
	glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, 0);

	// render right eye (second half of index array )
	glBindTexture(GL_TEXTURE_2D, rightEyeDesc.m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, (const void *)(uintptr_t)(m_uiCompanionWindowIndexSize));

	glBindVertexArray(0);
	glUseProgram(0);
}
