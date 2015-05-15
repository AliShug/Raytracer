#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "DirLight.h"
#include <memory>
#include <glm\gtc\matrix_transform.hpp>
#include <SDL.h>
#include <random>
#include <time.h>

Scene::Scene() {
}

Scene::~Scene() {
	if (_root) delete _root;
	if (_camera) delete _camera;
}

void Scene::SimpleScene() {
	_root = new SceneObj();
	_camera = new Camera(glm::vec3(4, 3, 2), { 0, 0, 0 });

	// A couple of materials
	Material *defaultMat = new Material();

	Material *shiny = new Material();
	shiny->diffuse = { 1, 0, 0 };
	shiny->spec = { 1.2f, 1.2f, 1.2f };
	shiny->specPow = 200;
	shiny->reflectivity = 1.0f;
	shiny->refractiveIndex = 4.0f;

	Material *shiny2 = new Material();
	shiny2->diffuse = { 0.8f, 0.8f, 0 };
	shiny2->spec = { 1.2f, 1.2f, 1.2f };
	shiny2->specPow = 200;
	shiny2->reflectivity = 1.0f;
	shiny2->refractiveIndex = 2.0f;

	// Add some objects
	SceneObj *plane = new Plane();
	SceneObj *sphere = new Sphere();
	SceneObj *sphere2 = new Sphere();
	SceneObj *sphere3 = new Sphere();

	plane->mat = defaultMat;
	sphere->mat = shiny;
	sphere2->mat = defaultMat;
	sphere3->mat = shiny2;

	_root->AddChild(plane);
	_root->AddChild(sphere);
	_root->AddChild(sphere2);
	_root->AddChild(sphere3);

	sphere->localM = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 1));
	sphere2->localM = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.8f, 0));
	sphere3->localM = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.2f, 0));
	sphere3->localM = glm::scale(sphere3->localM, glm::vec3(2.0f, 2.0f, 2.0f));
}

void Scene::RandomBalls() {
	// Add some directional lights
	//lights.push_back(new DirLight(glm::vec3(0.4f), { -1, -1, -1 }));
	//lights.push_back(new DirLight(glm::vec3(0.6f), { -1, 1, -1 }));

	int nBalls = 10;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> randDist(-1, 1);
	std::uniform_real_distribution<float> randFloat(0, 1);
	gen.seed(time(NULL));

	_root = new SceneObj();
	_camera = new Camera({ 4, 0.5f, 2 }, { 0, 0, 0 });

	for (int i = 0; i < nBalls; i++) {
		SceneObj *ball = new Sphere();
		Material *mat = new Material();
		mat->diffuse = { randFloat(gen), randFloat(gen), randFloat(gen) };
		mat->reflectivity = 0.2f;
		mat->refractiveIndex = 2.0f;
		mat->specPow = 200;
		ball->mat = mat;

		ball->localM = glm::translate(glm::mat4(1.0f), glm::vec3(
			randDist(gen)*2.0f,
			randDist(gen)*2.0f,
			randDist(gen)*2.0f));
		ball->localM = glm::scale(ball->localM, glm::vec3(randFloat(gen) * 2));

		_root->AddChild(ball);
	}
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