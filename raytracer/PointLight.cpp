#include "PointLight.h"


PointLight::PointLight(glm::vec3 col, glm::vec3 pos, float range) {
	_col = col;
	_pos = pos;
	_range = range;
}

PointLight::~PointLight() {}

LightInfo PointLight::CalcLightingInfo(const glm::vec3 &pt) {
	LightInfo li;

	li.dir = glm::normalize(_pos - pt);
	li.shadowDist = glm::distance(_pos, pt);
	if (li.shadowDist >= _range) li.col = glm::vec3(0.0f);
	else li.col = _col * (1.0f - li.shadowDist / _range);

	return li;
}
