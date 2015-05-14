#include "Camera.h"
#include "Application.h"


Camera::Camera(glm::vec3 pos, glm::vec3 targ) {
	_pos = pos;
	_targ = targ;

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
	_u = _aspect * glm::normalize(glm::cross(_w, glm::vec3(0, 1, 0)));
	_v = glm::normalize(glm::cross(_w, _u));
}

Ray Camera::GeneratePrimary(glm::vec2 screenPos) {
	// Four corners
	/*glm::vec3 ul, ur, ll, lr, p;

	ul.z = ur.z = ll.z = lr.z = 0;
	ul.x = -_aspect; ul.y = -1;
	ur.x =  _aspect; ur.y = -1;
	ll.x = -_aspect; ll.y =  1;
	lr.x =  _aspect; lr.y =  1;

	float u = screenPos.x, v = screenPos.y;
	p = u * (v*ll + (1 - v)*ul) + (1 - u)*(v*lr + (1 - v)*ur);

	p = p * 2.0f;

	Ray output;
	output.p = _pos;
	output.dir = p - _pos;
	output.dir = glm::normalize(output.dir);*/
	//output.p = p;

	Ray output;
	output.p = _pos;

	// Calculate the direction vector based on UVW system
	float length = 2.0f;
	float x = screenPos.x * 2 - 1, y = screenPos.y * 2 - 1;
	output.dir = glm::normalize(length*_w + x*_u + y*_v);

	return output;
}