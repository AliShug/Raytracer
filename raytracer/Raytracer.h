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
	SDL_PixelFormat* _pixelFormat = NULL;

	// Map floating-point colour to SDL color
	Uint32 MapCol(float r, float g, float b, float a = 1.0f) {
		return SDL_MapRGBA(_pixelFormat, r * 255, g * 255, b * 255, a * 255);
	}
};

