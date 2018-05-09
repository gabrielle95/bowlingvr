#include <GL/glew.h>
#include "Application.h"

void Application::SetState(GameStateInterface *state)
{
	if (this->state != nullptr)
	{
		this->state->Destroy();
		delete this->state;
	}
	this->state = state;
	this->state->Init();

#ifndef _DEBUG
	FreeConsole();
#endif
}

bool Application::IsWindowActive()
{
	return this->windowFocused;
}

unsigned int Application::w()
{
	return this->width;
}

unsigned int Application::h()
{
	return this->height;
}

vr::IVRSystem * Application::getVRpointer()
{
	return this->vr_pointer;
}

Application::Application(vr::IVRSystem *vr_pointer)
{
	try
	{
		this->vr_pointer = vr_pointer;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw application_exception(SDL_GetError());
		}
		else
		{
			this->width = 1920;
			this->height = 1080;

			if (vr_pointer != NULL)
			{
				//companion VR window
				this->window = new Window("VR Bowling Companion Window ", this->width, this->height);
			}
			else
			{
				//normal window
				this->window = new Window("VR Bowling", this->width, this->height);
			}

			if (glewInit() != GLEW_OK)
			{
				std::cout << "Error initializing GLEW!" << std::endl;
			}
			else
			{
				glEnable(GL_MULTISAMPLE);
			}

		}
	}
	catch(application_exception& exception)
	{
#ifdef _DEBUG
		std::cout << "Application exception at line: 22 in: Application.cpp ";
#else
		std::cout << "Application exception: ";
#endif
		std::cout << exception.what() << std::endl;
	}
	catch (window_exception& exception)
	{
#ifdef _DEBUG
		std::cout << "Window exception at line: 5 in: Window.cpp ";
#else
		std::cout << "Application exception: ";
#endif
		std::cout << exception.what() << std::endl;
	}
	catch (context_exception& exception)
	{
#ifdef _DEBUG
		std::cout << "GL Context exception in: Window.cpp ";
#else
		std::cout << "Application exception: ";
#endif
		std::cout << exception.what() << std::endl;
	}
}

bool Application::Run()
{
	if (this->active)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				this->Stop();
				break;
			case SDL_APP_TERMINATING:
				this->Stop();
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_FOCUS_LOST:
						std::cout << "SDL::Focus lost." << std::endl;
						this->windowFocused = false;
						SDL_ShowCursor(1);
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						std::cout << "SDL::Focus gained." << std::endl;
						this->windowFocused = true;
						SDL_ShowCursor(0);
						break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					this->Stop();
					break;
				}
				break;
			}
		}			

		if (this->state != nullptr)
		{
			this->state->Update();
		}
		this->window->Update();
	}
	

	return this->active;
}

void Application::Stop()
{
	this->active = false;
	delete this->state;
	this->state = nullptr;
}

Application::~Application()
{
	SDL_Quit();
}