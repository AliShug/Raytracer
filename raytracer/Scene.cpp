#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include <memory>
#include <glm\gtc\matrix_transform.hpp>


Scene::Scene() {
}

Scene::~Scene() {
	if (_root) delete _root;
	if (_camera) delete _camera;
}

void Scene::SimpleScene() {
	_root = new SceneObj();
	_camera = new Camera(glm::vec3(-3, 0.1f, 2), { 0, 0, 0 });

	// A couple of materials
	Material *defaultMat = new Material();
	Material *shiny = new Material();
	shiny->diffuse = { 1, 0, 0 };
	shiny->spec = { 1.2f, 1.2f, 1.2f };
	shiny->specPow = 200;
	shiny->opacity = 1.0f;
	shiny->refractiveIndex = 4.0f;

	// Add some objects
	SceneObj *plane = new Plane();
	SceneObj *sphere = new Sphere();
	plane->mat = defaultMat;
	sphere->mat = shiny;

	_root->AddChild(plane);
	_root->AddChild(sphere);

	sphere->localM = glm::translate(glm::mat4(1), glm::vec3(2, 2, 2));
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