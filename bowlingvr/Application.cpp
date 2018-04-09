#include <GL/glew.h>
#include "Application.h"

/*Application *Application::applicationInstance = nullptr;

Application* Application::Instance()
{
	if (Application::applicationInstance == nullptr)
	{
		Application::applicationInstance = new Application();
	}
	return Application::applicationInstance;
}*/

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

Window * Application::GetMainWindowPtr()
{
	return this->window;
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

IVRSystem * Application::getVRpointer()
{
	return vr_system->getVRpointer();
}

Application::Application()
{
	try
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw application_exception(SDL_GetError());
		}
		else
		{
			this->width = 1920;
			this->height = 1080;

			this->vr_system = new bVRSystem();
			vr_pointer = vr_system->getVRpointer();
			if (vr_pointer != NULL)
			{
				//companion VR window
				this->window = new Window("VR Bowling Companion Window", this->width, this->height);
			}
			else
			{
				//normal window
				std::cout << "OPENVR::Unable to init VR runtime: " << VR_GetVRInitErrorAsEnglishDescription(vr_system->getbVRError()) << std::endl;
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

			if (vr_pointer != NULL)
			{
				if (!vr_system->bVRInitVRCompositor())
				{
					throw bVRSystem_exception(VR_GetVRInitErrorAsEnglishDescription(vr_system->getbVRError()));
				}
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
	catch (bVRSystem_exception& exception)
	{
#ifdef _DEBUG
		std::cout << "VRCompositor exception in: bVRSystem.cpp ";
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

		//check for vr poll
		if(vr_pointer != NULL)
			bPollVREvent();

		if (this->state != nullptr)
		{
			this->state->Update();
		}
		this->window->Update();
	}
	

	return this->active;
}

void Application::bPollVREvent()
{
	VREvent_t event;
	if (vr_pointer->PollNextEvent(&event, sizeof(event)))
	{
		switch (event.eventType)
		{
		case VREvent_TrackedDeviceActivated:
			printf("EVENT (OpenVR) Device : %d attached\n",
				event.trackedDeviceIndex);
			break;

			//and so on, can test for any amount of vr events

		default:
			printf("EVENT--(OpenVR) Event: %d\n", event.eventType);
		}
	}
	//Output tracking data or do other things
}

void Application::Stop()
{
	this->active = false;
}

Application::~Application()
{
	delete this->state;
	if (vr_pointer != NULL)
	{
		vr_system->bVR_Shutdown();
		vr_pointer = NULL;
	}
	SDL_Quit();
}