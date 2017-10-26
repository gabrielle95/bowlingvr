#include "TestState.h"
#include "TestState2.h"

TestState::TestState(Application *application) : GameState(application)
{
}

bool TestState::Init()
{
	glClearColor(0,0,1,1);
	return true;
}

bool TestState::Update()
{
	glClear(GL_COLOR_BUFFER_BIT);

	const uint8_t *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RIGHT])
	{
		SetState<TestState2>(this->application);
	}
	return true;
}

bool TestState::Destroy()
{
	return true;
}

TestState::~TestState()
{
}