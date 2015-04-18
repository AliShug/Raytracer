#pragma once
#include <glm\glm.hpp>
#include "Ray.h"

class SceneObj {
public:
	SceneObj();
	~SceneObj();

	// Returns the object's world position
	glm::vec3 Position();
	// Calculate an intersection with a ray
	bool Intersect(const Ray &ray);

private:
	float _radius = 0.6f;
	glm::mat4 _local;
};

