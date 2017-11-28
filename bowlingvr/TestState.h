#pragma once
#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "Application.h"
#include "TestShape.h"
#include "Room.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "Camera.h"
#include "BulletWorld.h"
//#include "DynamicObj.h"

class TestState : public GameState
{
public:
	TestState(Application * application);
	bool Init();
	bool Update();
	bool Destroy();
	void ShootSphere(btVector3 direction);
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
	const float mouse_speed = 0.1f;
	GLint hasTextureUniform;

	uint32_t currentTime = 0;
	uint32_t lastTime = 0;
	float deltaTime;
};
#endif