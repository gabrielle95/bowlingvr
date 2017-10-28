#pragma once
#include "Application.h"
#include "TestShape.h"
#include "Shader.h"

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
};