#pragma once
#include "Application.h"
#include "TestShape.h"
#include "Room.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "Camera.h"
#include "BulletWorld.h"

class TestState : public GameState
{
public:
	TestState(Application * application);
	bool Init();
	bool Update();
	bool Destroy();
	~TestState();
private:
	Shader *shader;
	TestShape *testShape;
	Room *room;
	Camera *camera;
	BulletWorld *bwInstance;
	ObjLoader *sphereObj;

	//BulletObject *physObject;
	glm::vec3 camRotation;
	glm::vec3 camVelocity;
	const float mouse_speed = 0.1f;
	GLint hasTextureUniform;

	uint32_t currentTime = 0;
	uint32_t lastTime = 0;
	float deltaTime;
};