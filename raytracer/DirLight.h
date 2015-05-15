#pragma once
#include "Light.h"

class DirLight : public Light {
public:
	DirLight(glm::vec3 col, glm::vec3 dir);
	~DirLight();

	glm::vec3 CalcCol(const glm::vec3 &pt);
	glm::vec3 CalcDir(const glm::vec3 &pt);

protected:
	glm::vec3 _dir;
};

