#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include <memory>


Scene::Scene() {
}

Scene::~Scene() {
	if (_root) delete _root;
	if (_camera) delete _camera;
}

void Scene::SimpleScene() {
	_root = new SceneObj();
	_camera = new Camera(glm::vec3(0, 1, 3), { 0.4f, 0.4f, 0 });

	// Add some objects
	_root->AddChild(new Plane());
	_root->AddChild(new Sphere());
}

HitInfo Scene::Raycast(const Ray &ray) {
	return Raycast(ray, _root);
}

HitInfo Scene::Raycast(const Ray &ray, SceneObj *obj) {
	HitInfo out;
	float depth = INFINITY;

	for (int i = 0; i < obj->children.size(); i++) {
		HitInfo obHit = obj->children[i]->Intersect(ray);
		if (obHit.dist < depth) {
			depth = obHit.dist;
			out = obHit;
		}
	}

	return out;
}