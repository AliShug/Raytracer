#pragma once
#include <glm\glm.hpp>
#include "Ray.h"

class Camera {
public:
	Camera(glm::vec3 pos, glm::vec3 targ = { 0, 0, 0 });
	~Camera();

	// Generate a primary ray for the given screen coord
	Ray GeneratePrimary(glm::vec2 screenPos);

private:
	void CalcUVW();

	glm::vec3 _pos, _targ, _u, _v, _w;
	float _aspect;
};

