#pragma once
#include "SceneObj.h"
#include "Camera.h"
#include "Light.h"
#include <vector>

class Scene {
public:
	std::vector<Light*> lights;

	Scene();
	~Scene();

	// Create a scene
	void SimpleScene();

	// Scene of random assorted spheres
	void RandomBalls();

	// Tri test scene
	void TriScene();

	// Cast a ray in the scene
	HitInfo Raycast(const Ray &ray);
	HitInfo Raycast(const Ray &ray, SceneObj* obj);
	Camera* GetCamera() { return _camera; }

private:
	SceneObj* _root = nullptr;
	Camera* _camera = nullptr;
};

