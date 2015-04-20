#include "Ray.h"

Ray::Ray() {}
Ray::Ray(glm::vec3 pos, glm::vec3 direction) {
	p = pos;
	dir = glm::normalize(direction);
}

Ray::~Ray() {}
