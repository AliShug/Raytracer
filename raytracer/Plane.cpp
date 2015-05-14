#include "Plane.h"

Plane::Plane(glm::vec3 point, glm::vec3 normal) {
	_p = point;
	_n = normal;
}

Plane::~Plane() {}


HitInfo Plane::Intersect(const Ray &ray) {
	// Based on code from "Raytracing From the Ground up"
	HitInfo out;
	float t = glm::dot(_p - ray.p, _n) / glm::dot(ray.dir, _n);

	if (t > 0) {
		out.dist = t;
		out.hit = true;
		out.p = ray.p + t*ray.dir;
		out.n = _n;
	}

	return out;
}