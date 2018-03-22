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

class TestState : public GameState
{
public:
	TestState(Application * application);
	bool Init();
	bool Update();
	bool Destroy();
	void ShootSphere(btVector3 direction, btVector3 origin);
	~TestState();

private:
	Shader *modelShader = nullptr;
	Shader *depthShader = nullptr;
	Shader *hdrShader = nullptr;
	Shader *emissionShader = nullptr;
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

	Model *sphere;
	Alley *room;
	Model *pin;
	Alley *alley;

	PlayerBody *Player = nullptr;

	std::vector<Pin*> pins;
	std::vector<Box*> walls;

	std::vector<Model*>dynamicObjects;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	glm::vec3 camRotation;
	glm::vec3 camVelocity;
	const float mouse_speed = 2.f;
	float deltaTime = 0.f;
	float deltaNow = 0.f;
	float deltaThen = 0.f;
	GLint hasTextureUniform;

	uint32_t currentTime = 0;
	uint32_t lastTime = 0;

	btVector3 pt;

	bool debugMode = false;
	bool pressedP = false;
	bool pressedC = false;
	bool pressedSpace = false;

	//mouse position
	int xPos, yPos;
	GLint err;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int quadEBO;
	std::vector<float> quadvertices;
	std::vector<unsigned int>quadindices;

	float exposure = 2.5f;
	bool bloom = true;

	void RenderObjects(Shader *shader);
	void RenderLights(Shader *shader);
	void RenderQuad();
};