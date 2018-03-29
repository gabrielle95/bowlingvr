#include "MainScene.h"
#include "TestState2.h"


TestState2::TestState2(Application *application) : GameState(application)
{
}

bool TestState2::Init()
{
	glClearColor(0, 1, 0, 1);
	return true;
}

bool TestState2::Update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	const uint8_t *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
	{
		SetState<MainScene>(this->application);
	}
	return true;
}

bool TestState2::Destroy()
{
	return true;
}

TestState2::~TestState2()
{
}