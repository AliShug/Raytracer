#include "Camera.h"
#include "Application.h"


Camera::Camera(glm::vec3 pos, glm::vec3 targ, glm::vec3 up, float length) {
	_pos = pos;
	_targ = targ;
	_up = up;
	_length = length;

	int w, h;
	Application *app = Application::currentApp;
	SDL_Window *win = app->GetWindow();
	SDL_GetWindowSize(win, &w, &h);

	_aspect = (float) w / (float) h;
	CalcUVW();
}

Camera::~Camera() {}

void Camera::CalcUVW() {
	_w = glm::normalize(_targ - _pos);
	// Bake aspect into U vector
	_u = _aspect * glm::normalize(glm::cross(_w, _up));
	_v = glm::normalize(glm::cross(_w, _u));
}

Ray Camera::GeneratePrimary(glm::vec2 screenPos) {
	Ray output;
	output.p = _pos;

	// Calculate the direction vector based on UVW system
	float x = screenPos.x * 2 - 1, y = screenPos.y * 2 - 1;
	output.dir = glm::normalize(_length*_w + x*_u + y*_v);

	return output;
}