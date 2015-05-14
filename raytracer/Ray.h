#pragma once
#include <glm\glm.hpp>

class SceneObj;

class Ray {
public:
	Ray();
	Ray(glm::vec3 pos, glm::vec3 direction);
	~Ray();

	glm::vec3 p;
	glm::vec3 dir;
};

struct HitInfo {
	bool hit = false;
	float dist = INFINITY;
	glm::vec3 p = { 0, 0, 0 };
	glm::vec3 n = { 0, 0, 0 };
	SceneObj* obj = nullptr;
};