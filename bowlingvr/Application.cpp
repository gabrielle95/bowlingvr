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
			this->window = new Window("VR Bowling", 1600, 900); //////////
			if (glewInit() != GLEW_OK)
			{
				std::cout << "Error initializing GLEW!" << std::endl;
			}
			else
			{
				glEnable(GL_DEPTH_TEST);
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
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					this->Stop();
					break;
				case SDLK_SPACE:
					
					break;
				}
				break;
			}
		}
		/*glClearColor(0, 0.7, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);*/
		
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
}

Application::~Application()
{
	delete this->state;
	SDL_Quit();
}