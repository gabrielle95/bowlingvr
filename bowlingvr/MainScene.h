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
#include "libs/shared/Matrices.h"

//! Class of the main game level. 
/*!
Handles shader and model loading, component initialisation and keyboard input in non-vr mode.
*/
class MainScene : public GameState
{
public:
	MainScene(Application * application);

	/**
	* Initialises the scene.
	*/
	bool Init();

	/**
	* Renders a frame and updates the scene, steps simulation.
	*/
	bool Update();
	bool Destroy();

	/**
	*Throws a sphere by pressing C im non-VR mode.
	*/
	void ShootSphere(btVector3 direction, btVector3 origin);

	/**
	*Renders the OpenGL visualisation.
	*/
	void RenderScene();

	~MainScene();

protected:
	//entities
	Shader *modelShader = nullptr;
	Shader *depthShader = nullptr;
	Shader *hdrShader = nullptr;
	Shader *blurShader = nullptr;
	Shader *bloomShader = nullptr;
	
	Camera *tvCamera = nullptr;
	Camera *camera = nullptr;
	btDynamicsWorld *dynamicWorld = nullptr; //bulletPhysics

	std::vector<Light *> Lights;

	Shadowmap *CubeDepthMap = nullptr;
	
	GeneralFramebuffer *tvEffect = nullptr;

	PostProcessing *NoEffects = nullptr;
	PostProcessing *BloomEffect = nullptr;
	Hdr *HdrEffect = nullptr;
	MSAA *msaaEffect = nullptr;

	//models
	Model *sphere;
	Alley *room;
	Model *pin;

	PlayerBody *Player = nullptr;

	std::vector<Ball*> balls;
	std::vector<btVector3> ballPositions;

	std::vector<btVector3> pinPositions;
	std::vector<Pin*> pins;

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

	//tv scene quad
	unsigned int tvVAO = 0;
	unsigned int tvVBO;
	unsigned int tvEBO;
	std::vector<float> tvvertices;
	std::vector<unsigned int> tvindices;

	//functions
	void GetInputCallback();
	void RenderObjects(Shader *shader);
	void RenderLights(Shader *shader);
	void RenderQuad();
	void RenderTVSceneQuad();
};