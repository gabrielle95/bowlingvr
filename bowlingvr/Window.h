#pragma once

#include <iostream>
#include <exception>
#include <SDL2/SDL.h>

#define WFLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

class window_exception : public std::runtime_error
{
public:
	window_exception(const std::string& error) : std::runtime_error(error) {}
};

class context_exception : public std::runtime_error
{
public:
	context_exception(const std::string& error) : std::runtime_error(error) {}
};

class Window
{
public:
	Window(const std::string& title, int width, int height);
	Window(const std::string& title, int x, int y, int width, int height);
	void Update();
	~Window();

private:
	void InitRenderer();
	SDL_Window* sdlWindow;
	SDL_GLContext sdlGLContext;
};