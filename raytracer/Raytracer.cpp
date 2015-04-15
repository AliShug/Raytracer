#include <chrono>
#include <random>
#include "Raytracer.h"
#include "Application.h"

Raytracer::Raytracer() {
	_renderTexture = new RenderTexture();

	// Store the pixel format for use in colour-mapping
	_pixelFormat = SDL_GetWindowSurface(Application::currentApp->GetWindow())->format;
}
Raytracer::~Raytracer() {
	if (_renderTexture != NULL) {
		delete _renderTexture;
	}
}


void Raytracer::RenderStart() {
	// Retrieve the application object
	Application *app = Application::currentApp;

	// Get the window size
	int w, h;
	SDL_GetWindowSize(app->GetWindow(), &w, &h);

	// Create a blank RenderTexture
	_renderTexture->CreateTexture(app->GetRenderer(), SDL_GetWindowPixelFormat(app->GetWindow()), w, h);
}

void Raytracer::RenderStep() {
	// High-precision timing
	typedef chrono::system_clock Clock;

	// Random colour generator
	//static default_random_engine generator;
	//uniform_int_distribution<int> randomCol(0x00, 0xFF);

	// Retrieve the application
	Application* app = Application::currentApp;

	// Start the clock
	auto start = Clock::now();

	// Lock the renderTexture
	if (!_renderTexture->Lock()) {
		return;
	}

	Uint32* pixels = (Uint32*) _renderTexture->GetPixels();
	int n = _renderTexture->GetHeight() * (_renderTexture->GetPitch() / 4);

	// Staggered iteration over the image pixels (constant re-rendering)
	static int i = 0;
	for (; i < n; i++) {
		float x = (i % (_renderTexture->GetPitch() / 4)) / (float) _renderTexture->GetWidth();
		float y = (i / (_renderTexture->GetPitch() / 4)) / (float) _renderTexture->GetHeight();
		x = x * 2 - 1; y = y * 2 - 1;

		float val = (x + y) / 2;
		pixels[i] = MapCol(val, val, val);


		// Break out of pixel-fill after 10ms
		if (i % 1000 == 0) {
			auto current = Clock::now();
			auto time = chrono::duration_cast<chrono::milliseconds>(current - start);
			if (time.count() > 10) {
				//SDL_Log("%d\n", time.count());
				break;
			}
		}
	}
	if (i == n) i = 0;
	

	_renderTexture->Unlock();
}

void Raytracer::RenderEnd() {

}