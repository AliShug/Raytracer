#pragma once
#include <glm\glm.hpp>

class Material {
public:
	Material();
	~Material();

	glm::vec3 diffuse, spec;
	float specPow, reflectivity;
};
