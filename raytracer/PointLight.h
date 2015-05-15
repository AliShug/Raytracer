#pragma once
#include "Light.h"
class PointLight : public Light {
public:
	PointLight(glm::vec3 col, glm::vec3 pos, float range);
	~PointLight();

	LightInfo CalcLightingInfo(const glm::vec3 &pt);

protected:
	glm::vec3 _pos;
	float _range;
};

