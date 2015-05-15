#pragma once
#include <glm\glm.hpp>

class Light {
public:
	virtual glm::vec3 CalcDir(const glm::vec3 &pt) = 0;
	virtual glm::vec3 CalcCol(const glm::vec3 &pt) = 0;

protected:
	glm::vec3 _col;
};

