#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include <Windows.h>
#include <SDL2/SDL.h>
#include <GL/GL.h>

class application_exception : public std::runtime_error
{
public:
	application_exception(const std::string& error) : std::runtime_error(error){}
};

class GameStateInterface
{
public:
	virtual bool Init() = 0;
	virtual bool Update() = 0;
	virtual bool Destroy() = 0;
};

class Application
{
	static Application* applicationInstance;
public:
	Application();
	~Application();
	//static Application* Instance();
	bool Run();
	void Stop();
	void SetState(GameStateInterface *state);
	Window *GetMainWindowPtr();
private:
	bool active;
	Window* window;
	GameStateInterface *state = nullptr;
};

class GameState : public GameStateInterface
{
public:
	GameState(Application *application)
	{
		this->application = application;
	}
protected:
	Application *application;
};

template <class StateClass> void SetState(Application *application)
{
	application->SetState(new StateClass(application));
}

#endif