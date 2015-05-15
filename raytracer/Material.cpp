#include "Material.h"

// Default material
Material::Material() {
	diffuse = glm::vec3(1);
	spec = glm::vec3(0.3f);

	specPow = 10.0f;
	opacity = 1.0f;
	refractiveIndex = 1.0f;
	reflectivity = 0.0f;
}


Material::~Material() {}
