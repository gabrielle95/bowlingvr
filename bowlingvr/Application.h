#pragma once

#include "Window.h"
#include <Windows.h>
#include <SDL2/SDL.h>
#include <GL/GL.h>
#include <openvr/openvr.h>

//!  application_exception class. 
/*!
Handles application exceptions.
*/
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

//!  The core class of the program. 
/*!
Handles SDL events, OpenGL context creation and game states.
*/
class Application
{
	static Application* applicationInstance;
public:
	Application(vr::IVRSystem *vr_pointer);
	~Application();
	bool Run();
	void Stop();
	void SetState(GameStateInterface *state);
	bool IsWindowActive();
	unsigned int w();
	unsigned int h();
	vr::IVRSystem * getVRpointer();

private://VARIABLES
	bool active; //application active

	Window* window;
	GameStateInterface *state = nullptr;

	bool windowFocused;

	unsigned int width;
	unsigned int height;

	//! The VR system pointer.
	/*!
	Allows access to HTC Vive system, tracking data, etc.
	*/
	vr::IVRSystem *vr_pointer = NULL;

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

//!  This template function handles dependency injection. 
template <class StateClass> void SetState(Application *application)
{
	application->SetState(new StateClass(application));
}
