#include <glm\glm.hpp>
#include "Application.h"

Application* Application::currentApp = NULL;
Application::Application() {
	if (currentApp != NULL) {
		SDL_Log("More than one Application instance!");
	}

	appTitle = "Raytracer";
	currentApp = this;
}

Application::~Application() {
	currentApp = NULL;

	if (_raytracer != NULL || _window != NULL || _renderer != NULL) {
		SDL_Log("Application was not properly cleaned up - call Cleanup() upon exiting.");
		Cleanup();
	}
}

bool Application::Initialize() {
	// Try SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL_Init failed:\n%s", SDL_GetError());
		return false;
	}

	// Initialize the window
	_window = SDL_CreateWindow(	appTitle.c_str(),
								SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
								640, 480, SDL_WINDOW_SHOWN);

	if (_window == NULL) {
		SDL_Log("SDL_CreateWindow failed:\n%s", SDL_GetError());
		return false;
	}

	// Initialize the renderer
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (_renderer == NULL) {
		SDL_Log("SDL_CreateRenderer failed:\n%s", SDL_GetError());
		return false;
	}

	// Initialize the raytracer
	_raytracer = new Raytracer();
	_raytracer->RenderStart();

	// No bad things happened
	return true;
}

void Application::Cleanup() {
	// Raytracer is cleaned up before the SDL window & renderer
	if (_raytracer != NULL) {
		delete _raytracer;
		_raytracer = NULL;
	}

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	_window = NULL;
	_renderer = NULL;

	SDL_Quit();
}

void Application::HandleEvent(SDL_Event e) {
	// Handle input events!
}

void Application::PaintWindow() {
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);

	_raytracer->RenderStep();
	SDL_RenderCopy(_renderer, _raytracer->GetImage(), NULL, NULL);
	
	SDL_RenderPresent(_renderer);
}