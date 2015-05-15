#pragma once
#include "SceneObj.h"

class Triangle : public SceneObj {
public:
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	~Triangle();

	HitInfo Triangle::Intersect(const Ray &ray);

protected:
	glm::vec3 _v0, _v1, _v2, _n;
};

