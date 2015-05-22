#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "DirLight.h"
#include "PointLight.h"
#include <memory>
#include <glm\gtc\matrix_transform.hpp>
#include <SDL.h>
#include <random>
#include <time.h>

Scene::Scene() {
	_root = new SceneObj();
	_camera = new Camera(glm::vec3(0, 3, 3), { 1, 0, 0 });
}

Scene::~Scene() {
	if (_root) delete _root;
	if (_camera) delete _camera;
}

// Test scenes... //////////

void Scene::SimpleScene() {
	*_camera = Camera(glm::vec3(0, 3, 3), { 1, 0, 0 });

	lights.push_back(new DirLight(glm::vec3(1.0f), { 1, 1, -1 }));
	lights.push_back(new PointLight(glm::vec3(0.3f, 0.3f, 1.0f), { 2.5f, 0.5f, 1 }, 5.0f));

	// A couple of materials
	Material *defaultMat = new Material();

	Material *shiny = new Material();
	shiny->diffuse = glm::vec3(1.0f);
	shiny->spec = glm::vec3(1.2f);
	shiny->specPow = 200;
	shiny->reflectivity = 1.0f;
	shiny->refractiveIndex = 0.5f;
	shiny->opacity = 0.1f;

	Material *shiny2 = new Material();
	shiny2->diffuse = { 0.8f, 0.8f, 0.1f };
	shiny2->spec = { 1.2f, 1.2f, 1.2f };
	shiny2->specPow = 200;
	shiny2->reflectivity = 1.0f;
	shiny2->refractiveIndex = 1.8f;

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
	sphere2->localM = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0.8f, 0));
	sphere3->localM = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0.2f, 0));
	sphere3->localM = glm::scale(sphere3->localM, glm::vec3(2.0f, 2.0f, 2.0f));
}

void Scene::RandomBalls() {
	// Camera
	*_camera = Camera({ 5, 2, -2.5f }, { 0, 0.5f, 0 }, { 0, 1, 0 }, 2.0f);

	// Add some directional lights
	lights.push_back(new DirLight(glm::vec3(0.6f), { -1, 1, -1 }));
	//lights.push_back(new PointLight(glm::vec3(1.0f), { 0, 5, 0 }, 8.0f));
	//lights.push_back(new PointLight(glm::vec3(1.0f, 0.5f, 0.0f), { 5, 0, 0 }, 8.0f));
	//lights.push_back(new PointLight(glm::vec3(0.3f, 0.3f, 1.0f), { 0, 1, -3 }, 8.0f));
	lights.push_back(new PointLight(glm::vec3(1.0f, 0.3f, 0.3f), { 0, 0, 2 }, 8.0f));

	int nBalls = 9;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> randDist(-1, 1);
	std::uniform_real_distribution<float> randScale(0.4f, 1.0f);
	std::uniform_real_distribution<float> randFloat(0, 1);
	//gen.seed(time(NULL));
	gen.seed(42);

	for (int i = 0; i < nBalls; i++) {
		SceneObj *ball = new Sphere();
		Material *mat = new Material();
		mat->diffuse = { randFloat(gen), randFloat(gen), randFloat(gen) };
		mat->spec = glm::vec3(1.0f);
		mat->reflectivity = randFloat(gen);
		mat->refractiveIndex = 1.4f;
		mat->specPow = 20.0f;
		ball->mat = mat;

		ball->localM = glm::translate(glm::mat4(1.0f), glm::vec3(
			randDist(gen)*2.0f,
			1+randDist(gen),
			randDist(gen)*2.0f));
		ball->localM = glm::rotate(ball->localM, randFloat(gen), glm::vec3(1, 0.5f, 1));
		ball->localM = glm::scale(ball->localM, glm::vec3(randScale(gen), randScale(gen), randScale(gen)));

		_root->AddChild(ball);
	}

	SceneObj *plane = new Plane();
	plane->mat = new Material();
	_root->AddChild(plane);

	plane->mat->reflectivity = 1.0f;
	plane->mat->refractiveIndex = 1.3f;
}

void Scene::TriScene() {
	*_camera = Camera(glm::vec3(0, 0.5f, 3), { 0, 0.2f, 0 });

	lights.push_back(new DirLight(glm::vec3(1.0f), { 1, 1, 1 }));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 1.0f), { -1, 0.5f, 1 }, 5.0f));

	SceneObj *plane = new Plane(glm::vec3(0));
	plane->mat = new Material();
	plane->mat->reflectivity = 1.0f;
	plane->mat->refractiveIndex = 2.0f;

	SceneObj *tri = new Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 0.5f, 0.2f, 0.0f });
	tri->mat = new Material();
	tri->mat->refractiveIndex = 1.05f;

	_root->AddChild(plane);
	_root->AddChild(tri);
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