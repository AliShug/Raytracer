#pragma once
#include "SceneObj.h"

class Plane : public SceneObj {
public:
	Plane(glm::vec3 point = { 0, -0.8f, 0 }, glm::vec3 normal = { 0, 1, 0 });
	~Plane();

	HitInfo Intersect(const Ray &ray);

protected:
	glm::vec3 _p, _n;
};

