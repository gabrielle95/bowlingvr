#pragma once

#include "bVRRenderModel.h"
#include "ObjectPickingVR.h"

class MainSceneVR : public MainScene
{
public:
	MainSceneVR(Application * application);
	bool Init();
	bool Update();
	bool Destroy();

	Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);

	void SetupCameras();
	bool SetupStereoRenderTargets();
	void SetupCompanionWindow();
	void SetupRenderModels();
	void SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
	
	void RenderControllerAxes();
	void RenderToTV();
	void RenderStereoTargets();
	//void RenderScene(vr::Hmd_Eye nEye, Shader *controllerShader, Shader *renderModelShader);
	void RenderCompanionWindow();

	void RenderEyeQuad(vr::Hmd_Eye nEye);

	void ResetPositions();

	void RenderScene(vr::Hmd_Eye nEye);

	Matrix4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
	Matrix4 GetCurrentViewMatrix(vr::Hmd_Eye nEye);
	Matrix4 GetCurrentProjectionMatrix(vr::Hmd_Eye nEye);
	bVRRenderModel * FindOrLoadRenderModel(const char * renderModelString);

	~MainSceneVR();


	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};

private: //METHODS
	bool InitializeComponents();
	bool InitVRCompositor();
	bool InitRenderModels();
	bool InitEyeBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc);
	std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL);
	void UpdateHMDMatrixPose();

	Matrix4 glmFromSteamVRMatrix(const vr::HmdMatrix34_t & matPose);
	// per-loop
	void PollVREvent();

	void ProcessButtonEvent(vr::VREvent_t event);

	btVector3 getControllerPosition(vr::TrackedDeviceIndex_t trackedDeviceIndex);

	btVector3 getControllerRaycastDirection(vr::TrackedDeviceIndex_t trackedDeviceIndex);

private: //VARIABLES

	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	// VR System
	Application * application = nullptr;
	vr::IVRSystem * pHmd = NULL; // pointer to HMD
	vr::EVRInitError eError = vr::VRInitError_None;
	vr::IVRRenderModels *m_pRenderModels = NULL;
	bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];

	// PICKING
	ObjectPickingVR *Picker = nullptr;

	// VR Shaders
	Shader *VRcontrollerShader = nullptr;
	Shader *VRrendermodelShader = nullptr;
	Shader *VRcompanionwindowShader = nullptr;

	Shader *tvShader = nullptr;

	// VR Cameras
	Camera *leftEyeCamera = nullptr;
	Camera *rightEyeCamera = nullptr;

	// VR FBOS for EYES
	PostProcessing *leftEyeFBO = nullptr;
	PostProcessing *rightEyeFBO = nullptr;

	// VR TRACKED DEVICES
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	// VR RENDER MODELS
	std::vector < bVRRenderModel *> m_vecRenderModels;
	bVRRenderModel *m_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];
	
	// VR CONTROLLERS
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;

	GLuint m_glControllerVertBuffer;
	GLuint m_unControllerVAO;
	unsigned int m_uiControllerVertcount;

	// RECCOMENDED RESOLUTION
	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	// HMD Pose Matrix
	Matrix4 m_mat4HMDPose;

	Matrix4 m_mat4ProjectionLeft;
	Matrix4 m_mat4ProjectionRight;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4eyePosRight;

	//matrix locations
	unsigned int m_vr_controllerlocation;
	unsigned int m_vr_rendermodellocation;

	// COMPANION WINDOW related
	struct VertexDataWindow
	{
		Vector2 position;
		Vector2 texCoord;

		VertexDataWindow(const Vector2 & pos, const Vector2 tex) : position(pos), texCoord(tex) {	}
	};

	GLuint m_unCompanionWindowVAO;
	GLuint m_glCompanionWindowIDVertBuffer;
	GLuint m_glCompanionWindowIDIndexBuffer;
	unsigned int m_uiCompanionWindowIndexSize;

	Matrix4 view;

	// OBJECT PICKING
	vr::TrackedDeviceIndex_t pickCtrlIndex;
	bool bTriggerDown = false;
	bool bHit = false;


};