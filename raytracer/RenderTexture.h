#pragma once
#include <SDL.h>

class RenderTexture {
public:
	RenderTexture();
	~RenderTexture();

	// Initialize the render target texture
	bool CreateTexture(SDL_Renderer* renderer, Uint32 fmt, int w, int h);
	// Dealloc
	void Free();

	// Return the texture width in pixels
	int GetWidth() { return _width; }
	// Return the texture height in pixels
	int GetHeight() { return _height; }
	// Return the texture's pitch
	int GetPitch() { return _pitch; }
	// Return the texture's pixel format
	Uint32 GetFormat() { return _fmt; }

	// Color modulation
	void SetColor(Uint8 r, Uint8 g, Uint8 b);
	// Blending mode
	void SetBlendMode(SDL_BlendMode blending);
	// Alpha modulation
	void SetAlpha(Uint8 a);

	// Lock the pixel data for manipulation
	bool Lock();
	// Unlock the texture for display/use
	bool Unlock();
	// Retrieve the pixel data
	void* GetPixels() { return _data; }
	// Retrieve the texture handle
	SDL_Texture* GetTexture() { return _texture; }

private:
	int _width = 0, _height = 0, _pitch = 0;
	Uint32 _fmt;
	SDL_Texture* _texture = NULL;
	void* _data = NULL;
};

