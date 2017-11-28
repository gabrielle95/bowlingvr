#pragma once

#ifndef TESTSTATE2_H
#define TESTSTATE2_H
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
#endif