#pragma once

#include "Application.h"

class TestState2 : public GameState
{
public:
	TestState2(Application *application);
	bool Init();
	bool Update();
	bool Destroy();
	~TestState2();
};