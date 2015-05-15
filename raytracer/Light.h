#pragma once
#include <glm\glm.hpp>

struct LightInfo {
	glm::vec3 dir, col;
	float shadowDist = INFINITY;
};

class Light {
public:
	virtual LightInfo CalcLightingInfo(const glm::vec3 &pt) = 0;

protected:
	glm::vec3 _col;
};

