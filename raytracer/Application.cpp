#include "Application.h"

Application::Application() {
	appTitle = "Raytracer";
}
Application::~Application() {}

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

	// No bad things happened
	return true;
}

void Application::Cleanup() {
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);

	SDL_Quit();
}

void Application::HandleEvent(SDL_Event e) {
	// Handle input events!
}

void Application::PaintWindow() {
	SDL_RenderClear(_renderer);

	// Other things

	SDL_RenderPresent(_renderer);
}