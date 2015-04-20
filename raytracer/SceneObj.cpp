#include "SceneObj.h"
#include <glm\gtc\matrix_access.hpp>

SceneObj::SceneObj() {}
SceneObj::~SceneObj() {}

glm::vec3 SceneObj::Position() {
	glm::vec4 pos;
	pos = glm::column(_local, 3);
	return glm::vec3(pos);
}

bool SceneObj::Intersect(const Ray &ray) {
	/*Ray newRay = ray; // copy
	newRay.p.z = 0;

	if (glm::distance(newRay.p, Position()) < _radius) {
		return true;
	}*/

	// Sphere-ray intersection
	float a, b, c;
	a = _radius * _radius;
	b = 2 * glm::dot()

	return false;
}