#pragma once
#include "SceneObj.h"

class Sphere : public SceneObj {
public:
	Sphere();
	~Sphere();

	HitInfo Intersect(const Ray &ray);

protected:
	float _radius = 0.6f;
};

