#include "Camera.h"
#include "Application.h"


Camera::Camera(glm::vec3 pos) {
	_pos = pos;

	int w, h;
	Application *app = Application::currentApp;
	SDL_Window *win = app->GetWindow();
	SDL_GetWindowSize(win, &w, &h);

	_aspect = (float) w / (float) h;
}

Camera::~Camera() {}


Ray Camera::GeneratePrimary(glm::vec2 screenPos) {
	glm::vec3 ul, ur, ll, lr, p;

	ul.z = ur.z = ll.z = lr.z = 0;
	ul.x = -_aspect; ul.y = -1;
	ur.x =  _aspect; ur.y = -1;
	ll.x = -_aspect; ll.y =  1;
	lr.x =  _aspect; lr.y =  1;

	float u = screenPos.x, v = screenPos.y;
	p = u * (v*ll + (1 - v)*ul) + (1 - u)*(v*lr + (1 - v)*ur);

	Ray output;
	//output.p = _pos;
	//output.dir = p - _pos;
	output.p = p;

	return output;
}