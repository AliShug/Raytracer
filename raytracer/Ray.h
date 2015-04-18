#pragma once
#include <glm\glm.hpp>

class Ray {
public:
	Ray();
	Ray(glm::vec3 pos, glm::vec3 direction);
	~Ray();

	glm::vec3 p;
	glm::vec3 dir;
};

