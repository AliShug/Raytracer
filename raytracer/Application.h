#pragma once
#include <SDL.h>
#include <string>

using namespace std;

class Application
{
public:
	Application();
	~Application();

	// Application initialization; creates the window
	bool Initialize();
	// Cleans up the allocated resources
	void Cleanup();

	// Handles the given window event
	void HandleEvent(SDL_Event e);
	// Renders to the window
	void PaintWindow();

	string appTitle;

private:
	SDL_Window* _window = NULL;
	SDL_Renderer* _renderer = NULL;
};

