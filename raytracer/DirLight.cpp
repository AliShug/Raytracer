#include "DirLight.h"

DirLight::DirLight(glm::vec3 col, glm::vec3 dir) {
	_dir = glm::normalize(dir);
	_col = col;
}

DirLight::~DirLight() {}

glm::vec3 DirLight::CalcCol(const glm::vec3 &pt) {
	return _col;
}

glm::vec3 DirLight::CalcDir(const glm::vec3 &pt){
	return _dir;
}