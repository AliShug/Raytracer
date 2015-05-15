#include "Triangle.h"

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
	_v0 = v0;
	_v1 = v1;
	_v2 = v2;

	// Find the normal
	glm::vec3 a = v1 - v0;
	glm::vec3 b = v2 - v0;
	_n = glm::normalize(glm::cross(a, b));
}

Triangle::~Triangle() {}

HitInfo Triangle::Intersect(const Ray &ray) {
	// Based on code from "Raytracing From the Ground up"
	// Additional code performs simple geometric triangle intersection
	HitInfo out;
	float t = glm::dot(_v0 - ray.p, _n) / glm::dot(ray.dir, _n);

	if (t > 0.01f) {
		glm::vec3 e0, e1, e2, vp0, vp1, vp2, p;

		p = ray.p + t*ray.dir;

		e0 = _v1 - _v0;
		e1 = _v2 - _v1;
		e2 = _v0 - _v2;

		vp0 = p - _v0;
		vp1 = p - _v1;
		vp2 = p - _v2;

		// See if the point lies outside the triangle area
		if (glm::dot(_n, glm::cross(e0, vp0)) < 0) return out;
		if (glm::dot(_n, glm::cross(e1, vp1)) < 0) return out;
		if (glm::dot(_n, glm::cross(e2, vp2)) < 0) return out;

		// Otherwise we hit
		out.p = p;
		out.n = (glm::dot(_n, ray.dir) > 0) ? -_n : _n;
		out.dist = t;
		out.obj = this;
		out.hit = true;
	}

	return out;

	// Transform ray into object-space
	/*glm::vec4 dir4 = glm::vec4(ray.dir, 0.0f);
	glm::vec4 p4 = glm::vec4(ray.p, 1.0f);

	glm::mat4 invM = glm::inverse(localM);
	glm::vec3 p = glm::vec3(invM * p4);
	glm::vec3 dir = glm::vec3(invM * dir4);
	dir = glm::normalize(dir);

	HitInfo out;

	// Sphere-ray intersection (sphere at local origin)
	float rootVal = powf(glm::dot(dir, p), 2) -
		glm::dot(p, p) + _radius*_radius;

	if (rootVal >= 0) {
		// Calculate the hit pt
		float root = sqrtf(rootVal);
		float term = -glm::dot(dir, p);

		float d1 = term + root, d2 = term - root;

		// This logic could probably be better
		float d = 0;
		if (d1 < 0 && d2 < 0) {
			out.hit = false;
			return out;
		}
		else if (d1 > 0 && d1 < d2 || d2 < 0) {
			d = d1;
		}
		else {
			d = d2;
		}

		p4 = glm::vec4(p + dir*d, 1);
		p4 = localM*p4;
		out.p = glm::vec3(p4);
		out.dist = glm::distance(ray.p, out.p);
		out.n = glm::normalize(p + dir*d);
		out.hit = true;
		out.obj = this;
	}

	return out;*/
}