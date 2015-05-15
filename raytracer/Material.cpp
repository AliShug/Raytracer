#include "Material.h"

// Default material
Material::Material() {
	diffuse = glm::vec3(1);
	spec = glm::vec3(0.3f);

	specPow = 10.0f;
	reflectivity = 0.01f;
}


Material::~Material() {}
