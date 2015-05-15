#include "DirLight.h"

DirLight::DirLight(glm::vec3 col, glm::vec3 dir) {
	_dir = glm::normalize(dir);
	_col = col;
}

DirLight::~DirLight() {}

LightInfo DirLight::CalcLightingInfo(const glm::vec3 &pt) {
	LightInfo li;
	li.dir = _dir;
	li.col = _col;

	return li;
}