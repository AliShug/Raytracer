#include "Raytracer.h"
#include "Application.h"

Raytracer::Raytracer() {
	_renderTexture = new RenderTexture();
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
	//_renderTexture->
}

void Raytracer::RenderEnd() {

}