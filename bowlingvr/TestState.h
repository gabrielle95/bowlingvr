#pragma once
#include "Application.h"

class TestState : public GameState
{
public:
	TestState(Application * application);
	bool Init();
	bool Update();
	bool Destroy();
	~TestState();
};