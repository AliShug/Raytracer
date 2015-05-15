#pragma once
#include <glm\glm.hpp>
#include "Ray.h"
#include "Material.h"
#include <vector>

class SceneObj {
public:
	SceneObj();
	~SceneObj();

	std::vector<SceneObj*> children;

	void AddChild(SceneObj *obj) {
		children.push_back(obj);
		obj->_parent = this;
	}

	// Returns the object's world position
	virtual glm::vec3 Position();
	// Calculate an intersection with a ray
	virtual HitInfo Intersect(const Ray &ray) { return HitInfo(); }

	Material *mat;
	glm::mat4 localM;

protected:
	SceneObj *_parent;
};

