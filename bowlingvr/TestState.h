#pragma once
#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "Application.h"
#include "Model.h"
#include "Entities.h"
#include "PlayerBody.h"
#include "Shader.h"
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
	Shader *shader;
	Shader *modelShader;

	Camera *camera;
	btDynamicsWorld *dynamicWorld; //bulletPhysics

	Model *sphere;
	Alley *room;
	Model *pin;

	Alley *alley;

	PlayerBody *Player;

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
};
#endif