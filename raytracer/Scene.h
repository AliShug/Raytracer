#pragma once
#include "SceneObj.h"
#include "Camera.h"

class Scene {
public:
	Scene();
	~Scene();

	// Create a default scene (just one sphere)
	void SimpleScene();

	// Cast a ray in the scene
	HitInfo Raycast(const Ray &ray);
	HitInfo Raycast(const Ray &ray, SceneObj* obj);
	Camera* GetCamera() { return _camera; }

private:
	SceneObj* _root = nullptr;
	Camera* _camera = nullptr;
};

