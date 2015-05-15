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
	_camera = new Camera(glm::vec3(-3, 0.6f, -2), { 0, 0, 0 });

	// A couple of materials
	Material *defaultMat = new Material();
	Material *shiny = new Material();
	shiny->diffuse = { 0.7f, 0.2f, 0.2f };
	shiny->spec = { 1.0f, 1.0f, 1.0f };
	shiny->specPow = 40;
	shiny->reflectivity = 0.5f;

	// Add some objects
	SceneObj *plane = new Plane();
	SceneObj *sphere = new Sphere();
	plane->mat = defaultMat;
	sphere->mat = shiny;

	_root->AddChild(plane);
	_root->AddChild(sphere);
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