#pragma once
#include "RenderTexture.h"
#include "Scene.h"

class Raytracer {
public:
	Raytracer();
	~Raytracer();

	// Begin the rendering process
	void RenderStart(Scene* scene);
	// Render the image
	void RenderStep();
	// Finalise the render
	void RenderEnd();

	// Retrieve the current image
	SDL_Texture* GetImage() { return _renderTexture->GetTexture(); }

private:
	RenderTexture* _renderTexture = nullptr;
	SDL_PixelFormat* _pixelFormat = nullptr;

	Scene* _scene = nullptr;
	Camera* _camera = nullptr;

	// Shade a pixel
	glm::vec3 Shade(const Ray &ray, const HitInfo &hitInfo, int depth = 0);

	// Map floating-point colour to SDL color
	Uint32 MapCol(float r, float g, float b, float a = 1.0f) {
		r = (r < 0) ? 0 : (r > 1) ? 1 : r;
		g = (g < 0) ? 0 : (g > 1) ? 1 : g;
		b = (b < 0) ? 0 : (b > 1) ? 1 : b;
		return SDL_MapRGBA(_pixelFormat, r * 255, g * 255, b * 255, a * 255);
	}

	Uint32 MapCol(glm::vec3 col) {
		float r = col.r, g = col.g, b = col.b;
		r = (r < 0) ? 0 : (r > 1) ? 1 : r;
		g = (g < 0) ? 0 : (g > 1) ? 1 : g;
		b = (b < 0) ? 0 : (b > 1) ? 1 : b;
		return SDL_MapRGBA(_pixelFormat, r * 255, g * 255, b * 255, 255);
	}
};

