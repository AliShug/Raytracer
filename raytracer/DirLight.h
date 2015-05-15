#pragma once
#include "Light.h"

class DirLight : public Light {
public:
	DirLight(glm::vec3 col, glm::vec3 dir);
	~DirLight();

	LightInfo CalcLightingInfo(const glm::vec3 &pt);

protected:
	glm::vec3 _dir;
};

