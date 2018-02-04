#include "Window.h"

Window::Window(const std::string& title, int width, int height)
{
	this->sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, WFLAGS);
	if (this->sdlWindow == nullptr)
	{
		throw window_exception(SDL_GetError());
	}
	else
	{
		this->InitRenderer();
	}
}

Window::Window(const std::string& title, int x, int y, int width, int height)
{
	this->sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, WFLAGS);
	if (this->sdlWindow == nullptr)
	{
		throw window_exception(SDL_GetError());
	}
	else
	{
		//SDL_SetWindowGrab(this->sdlWindow, SDL_TRUE);
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		this->InitRenderer();
	}
}

void Window::InitRenderer()
{
	this->sdlGLContext = SDL_GL_CreateContext(this->sdlWindow);
	if (this->sdlGLContext == nullptr)
	{
		throw context_exception(SDL_GetError());
	}
	else
	{
		//VSYNC 1 on 0 off
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			throw context_exception(SDL_GetError());
		}
		IMG_Init(IMG_INIT_PNG);
	}

}

void Window::Update()
{
	SDL_GL_SwapWindow(this->sdlWindow);
}

SDL_Window * Window::GetSDLWindowPtr()
{
	return this->sdlWindow;
}

Window::~Window()
{
	SDL_GL_DeleteContext(this->sdlGLContext);
	SDL_DestroyWindow(this->sdlWindow);
}