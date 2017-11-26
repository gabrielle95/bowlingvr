#pragma once
#include "Application.h"
#include "TestShape.h"
#include "Room.h"
#include "Shader.h"
#include "Camera.h"
#include "BulletWorld.h"
#include "Box.h"

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

	Box *box;
	//BulletObject *physObject;
	glm::vec3 camRotation;
	glm::vec3 camVelocity;
	const float mouse_speed = 0.1f;
};