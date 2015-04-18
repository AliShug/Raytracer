#pragma once
#include <SDL.h>
#include <string>
#include "Raytracer.h"
#include "Scene.h"

using namespace std;

class Application
{
public:
	static Application* currentApp;

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

	// Return the main window renderer handle
	SDL_Renderer* GetRenderer() { return _renderer; }
	// Return the associated window handle
	SDL_Window* GetWindow() { return _window; }

	string appTitle;

private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	Raytracer* _raytracer = nullptr;
	Scene* _scene = nullptr;
	Camera* _camera = nullptr;
};
