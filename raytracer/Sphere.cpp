#include "Sphere.h"
#include <glm\gtc\matrix_transform.hpp>

Sphere::Sphere() {
	localM = glm::mat4(1.0f);
}

Sphere::~Sphere() {}

HitInfo Sphere::Intersect(const Ray &ray) {
	// Transform ray into object-space
	glm::vec4 dir4 = glm::vec4(ray.dir, 0.0f);
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

	return out;
}