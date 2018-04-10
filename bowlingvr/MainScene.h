#pragma once

#include "Application.h"
#include "Model.h"
#include "Entities.h"
#include "PlayerBody.h"
#include "Shader.h"
#include "Light.h"
#include "Shadowmap.h"
#include "PostProcessing.h"
#include "Camera.h"
#include "CDebugDraw.h"
#include "BulletWorld.h"
#include "bVRMainScene.h"

class MainScene : public GameState
{
public:
	MainScene(Application * application);
	bool Init();
	bool Update();
	bool Destroy();
	void ShootSphere(btVector3 direction, btVector3 origin);
	~MainScene();

private:

	//VR hmd
	IVRSystem * vr_pointer = nullptr;

	//VR-side bookkeeping
	bool isVRenabled = false;
	bVRMainScene *vr_scene = nullptr;
	

	GLint m_vr_controllerlocation;
	GLint m_vr_rendermodellocation;

	Shader *VRcontrollerShader = nullptr;
	Shader *VRrendermodelShader = nullptr;
	Shader *VRcompanionwindowShader = nullptr;

	//entities
	Shader *modelShader = nullptr;
	Shader *depthShader = nullptr;
	Shader *hdrShader = nullptr;
	Shader *blurShader = nullptr;
	Shader *bloomShader = nullptr;
	

	Camera *camera = nullptr;
	btDynamicsWorld *dynamicWorld = nullptr; //bulletPhysics

	std::vector<Light *> Lights;

	Shadowmap *CubeDepthMap = nullptr;
	
	PostProcessing *NoEffects = nullptr;
	PostProcessing *BloomEffect = nullptr;
	Hdr *HdrEffect = nullptr;
	MSAA *msaaEffect = nullptr;

	//models
	Model *sphere;
	Alley *room;
	Model *pin;
	//Model *tmppin;
	//Alley *alley;
	//Pin *testpin;

	PlayerBody *Player = nullptr;

	std::vector<Pin*> pins;
	std::vector<Box*> walls;

	std::vector<Model*>dynamicObjects;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	//player RB transform
	btTransform ptrans;

	//camera related variables
	glm::vec3 camRotation;
	glm::vec3 camVelocity;
	
	//some fps related variables
	const float mouse_speed = 2.f;
	float deltaTime = 0.f;
	float deltaNow = 0.f;
	float deltaThen = 0.f;

	//some keys
	bool debugMode = false;
	bool pressedP = false;
	bool pressedC = false;
	bool pressedSpace = false;

	//mouse position
	int xPos, yPos;
	GLint err;

	//post-processing quad
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int quadEBO;
	std::vector<float> quadvertices;
	std::vector<unsigned int>quadindices;

	//post processing variables
	float exposure = 2.5f;
	bool bloom = true;
	bool PP = true;

	//functions
	void GetInputCallback();
	void RenderObjects(Shader *shader);
	void RenderLights(Shader *shader);
	void RenderQuad();
};