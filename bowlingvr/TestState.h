#pragma once
#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "Application.h"
#include "TestShape.h"
#include "Room.h"
#include "ObjLoader.h"
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
	void drawLines(std::vector<CDebugDraw::LINE>& lines);
	void ShootSphere(btVector3 direction, btVector3 origin);
	~TestState();
private:
	Shader *shader;
	TestShape *testShape;
	Room *room;
	Camera *camera;
	btDynamicsWorld *dynamicWorld; //bulletPhysics

	ObjLoader *sphereObj;
	ObjLoader *smallSp;

	std::vector<ObjLoader*>dynamicObjects;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	glm::vec3 camRotation;
	glm::vec3 camVelocity;
	const float mouse_speed = 1.f;
	float deltaTime = 0.f;
	float deltaNow = 0.f;
	float deltaThen = 0.f;
	GLint hasTextureUniform;

	uint32_t currentTime = 0;
	uint32_t lastTime = 0;

	bool debugMode = false;
	bool pressedP = false;
	bool pressedC = false;
};
#endif