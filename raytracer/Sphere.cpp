#include "Sphere.h"


Sphere::Sphere() {}

Sphere::~Sphere() {}

HitInfo Sphere::Intersect(const Ray &ray) {
	HitInfo out;

	// Sphere-ray intersection (sphere at origin)
	float rootVal = powf(glm::dot(ray.dir, ray.p), 2) -
		glm::dot(ray.p, ray.p) + _radius*_radius;

	if (rootVal >= 0) {
		// Calculate the hit pt
		float root = sqrtf(rootVal);
		float term = -glm::dot(ray.dir, ray.p);

		float d1 = term + root, d2 = term - root;

		// This logic could probably be better
		if (d1 < 0 && d2 < 0) {
			out.hit = false;
			return out;
		}
		else if (d1 > 0 && d1 < d2 || d2 < 0) {
			out.dist = d1;
		}
		else {
			out.dist = d2;
		}

		out.p = ray.p + ray.dir*out.dist;
		out.n = glm::normalize(out.p);
		out.hit = true;
		out.obj = this;
	}

	return out;
}