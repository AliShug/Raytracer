#include <chrono>
#include <random>
#include "Raytracer.h"
#include "Application.h"

Raytracer::Raytracer() {
	_renderTexture = new RenderTexture();

	// Store the pixel format for use in colour-mapping
	_pixelFormat = SDL_GetWindowSurface(Application::currentApp->GetWindow())->format;
}
Raytracer::~Raytracer() {
	if (_renderTexture != NULL) {
		delete _renderTexture;
	}
}


void Raytracer::RenderStart(Scene* scene) {
	// Retrieve the application object
	Application *app = Application::currentApp;

	// Get the window size
	int w, h;
	SDL_GetWindowSize(app->GetWindow(), &w, &h);

	// Create a blank RenderTexture
	_renderTexture->CreateTexture(app->GetRenderer(), SDL_GetWindowPixelFormat(app->GetWindow()), w, h);

	// Save the scene/camera references
	_scene = scene;
	_camera = scene->GetCamera();
}


void Raytracer::RenderStep() {
	if (_scene == nullptr || _camera == nullptr) {
		SDL_Log("Scene/camera NULL in RenderStep() - must call RenderStart() with valid scene.");
		return;
	}

	// High-precision timing
	typedef chrono::system_clock Clock;

	// Random colour generator
	//static default_random_engine generator;
	//uniform_int_distribution<int> randomCol(0x00, 0xFF);

	// Retrieve the application
	Application* app = Application::currentApp;

	// Start the clock
	auto start = Clock::now();

	// Lock the renderTexture
	if (!_renderTexture->Lock()) {
		return;
	}

	Uint32* pixels = (Uint32*) _renderTexture->GetPixels();
	int n = _renderTexture->GetHeight() * (_renderTexture->GetPitch() / 4);

	// Staggered iteration over the image pixels (constant re-rendering)
	static int i = 0;
	for (; i < n; i++) {
		float x = (i % (_renderTexture->GetPitch() / 4)) / (float) _renderTexture->GetWidth();
		float y = (i / (_renderTexture->GetPitch() / 4)) / (float) _renderTexture->GetHeight();
		//x = x * 2 - 1; y = y * 2 - 1;

		Ray primary = _scene->GetCamera()->GeneratePrimary({ x, y });

		// Primary ray shading
		HitInfo hitInfo = _scene->Raycast(primary);
		if (hitInfo.hit) {
			pixels[i] = MapCol(Shade(primary, hitInfo));
		}
		else {
			// Sky
			if (primary.dir.y < 0) {
				pixels[i] = MapCol(0, 0, 0);
			}
			else {
				pixels[i] = MapCol(primary.dir.y, primary.dir.y, primary.dir.y);
			}
		}


		// Break out of pixel-fill after 10ms
		if (i % 1000 == 0) {
			auto current = Clock::now();
			auto time = chrono::duration_cast<chrono::milliseconds>(current - start);
			if (time.count() > 10) {
				//SDL_Log("%d\n", time.count());
				break;
			}
		}
	}
	if (i == n) i = 0;
	

	_renderTexture->Unlock();
}

glm::vec3 Raytracer::Shade(const Ray &ray, const HitInfo &hitInfo, int depth) {
	if (depth > 3) return{ 0, 0, 0 };

	if (!hitInfo.hit) {
		// Sky
		if (ray.dir.y < 0) {
			return { 0, 0, 0 };
		}
		else {
			return { ray.dir.y, ray.dir.y, ray.dir.y };
		}
	}

	// Simple directional light
	glm::vec3 lightDir = { -1, -1, -1 };
	lightDir = glm::normalize(lightDir);

	// Generate the shadow ray
	Ray shadowRay = Ray(hitInfo.p - 0.001f * lightDir, -lightDir);
	HitInfo shadowHit = _scene->Raycast(shadowRay);

	// General phong shading
	float ambient = 0.1f;
	float base = glm::dot(-lightDir, hitInfo.n);
	glm::vec3 reflect = glm::reflect(ray.dir, hitInfo.n);
	float spec = powf(glm::dot(-lightDir, reflect), 30);
	spec = (spec < 0) ? 0 : spec; // negative spec is bad spec

	// Reflections
	Ray reflectRay = Ray(hitInfo.p + 0.001f * reflect, reflect);
	HitInfo reflectHit = _scene->Raycast(reflectRay);
	glm::vec3 reflection = Shade(reflectRay, reflectHit, depth + 1);

	// Combine components according to shadowing
	if (shadowHit.hit) {
		return glm::vec3(ambient, ambient, 0) + reflection*0.3f;
	}
	else {
		return glm::vec3(base + spec + ambient, base + spec + ambient, spec) + reflection*0.3f;
	}
}

void Raytracer::RenderEnd() {
	_scene = nullptr;
	_camera = nullptr;
}