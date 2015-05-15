#include <glm\glm.hpp>
#include "Application.h"

Application* Application::currentApp = nullptr;
Application::Application() {
	if (currentApp != nullptr) {
		SDL_Log("More than one Application instance!");
	}

	appTitle = "Raytracer";
	currentApp = this;
}

Application::~Application() {
	currentApp = nullptr;

	if (_raytracer != nullptr || _window != nullptr || _renderer != nullptr) {
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

	if (_window == nullptr) {
		SDL_Log("SDL_CreateWindow failed:\n%s", SDL_GetError());
		return false;
	}

	// Initialize the renderer
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (_renderer == nullptr) {
		SDL_Log("SDL_CreateRenderer failed:\n%s", SDL_GetError());
		return false;
	}

	// Initialize the scene & camera
	_scene = new Scene();
	//_scene->SimpleScene();
	_scene->RandomBalls();

	// Initialize the raytracer
	_raytracer = new Raytracer();
	_raytracer->RenderStart(_scene);

	// No bad things happened
	return true;
}

void Application::Cleanup() {
	// Raytracer is cleaned up before the SDL window & renderer
	if (_raytracer != nullptr) {
		delete _raytracer;
		_raytracer = nullptr;
	}

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	_window = nullptr;
	_renderer = nullptr;

	SDL_Quit();
}

void Application::HandleEvent(SDL_Event e) {
	// Handle input events!
}

void Application::PaintWindow() {
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);

	_raytracer->RenderStep();
	SDL_RenderCopy(_renderer, _raytracer->GetImage(), nullptr, nullptr);
	
	SDL_RenderPresent(_renderer);
}