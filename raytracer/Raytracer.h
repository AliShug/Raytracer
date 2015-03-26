#pragma once
#include "RenderTexture.h"

class Raytracer {
public:
	Raytracer();
	~Raytracer();

	// Begin the rendering process
	void RenderStart();
	// Render the image
	void RenderStep();
	// Finalise the render
	void RenderEnd();

	// Retrieve the current image
	SDL_Texture* GetImage() { return _renderTexture->GetTexture(); }

private:
	RenderTexture* _renderTexture = NULL;
};

