#include "RenderTexture.h"

RenderTexture::RenderTexture() {}
RenderTexture::~RenderTexture() {
	Free();
}

bool RenderTexture::CreateTexture(SDL_Renderer* renderer, Uint32 fmt, int w, int h) {
	Free();
	_texture = SDL_CreateTexture(renderer, fmt, SDL_TEXTUREACCESS_STREAMING, w, h);

	if (_texture == NULL) {
		SDL_Log("Failed to create RenderTexture:\n%s", SDL_GetError());
		return false;
	}

	_width = w;
	_height = h;
	_fmt = fmt;
	return true;
}

void RenderTexture::Free() {
	if (_texture != NULL) {
		SDL_DestroyTexture(_texture);
		_texture = NULL;
		_data = NULL;
	}
}

bool RenderTexture::Lock() {
	if (_data != NULL) {
		SDL_Log("RenderTexture is already locked!");
		return false;
	}
	
	if (SDL_LockTexture(_texture, NULL, &_data, &_pitch) != 0) {
		SDL_Log("Unable to lock RenderTexture");
		return false;
	}

	return true;
}

bool RenderTexture::Unlock() {
	if (_data == NULL) {
		SDL_Log("RenderTexture was not locked");
		return false;
	}

	SDL_UnlockTexture(_texture);
	_data = NULL;
	_pitch = 0;

	return true;
}
