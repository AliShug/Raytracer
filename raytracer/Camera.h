#pragma once
#include <glm\glm.hpp>
#include "Ray.h"

class Camera {
public:
	Camera(glm::vec3 pos);
	~Camera();

	// Generate a primary ray for the given screen coord
	Ray GeneratePrimary(glm::vec2 screenPos);

private:
	glm::vec3 _pos;
	float _aspect;
};

