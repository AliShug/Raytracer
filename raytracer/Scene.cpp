#include "Scene.h"


Scene::Scene() {
}

Scene::~Scene() {
	if (_root) delete _root;
	if (_camera) delete _camera;
}

void Scene::SimpleScene() {
	_root = new SceneObj();
	_camera = new Camera(glm::vec3(0, 0, -10));
}

bool Scene::Raycast(const Ray &ray) {
	return _root->Intersect(ray);
}