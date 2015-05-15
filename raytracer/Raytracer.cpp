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
	static bool quickRendered = false;
	static bool rendered = false;

	if (rendered) return; // Completed rendering

	if (_scene == nullptr || _camera == nullptr) {
		SDL_Log("Scene/camera NULL in RenderStep() - must call RenderStart() with valid scene.");
		return;
	}

	// High-precision timing
	typedef chrono::system_clock Clock;

	// Random colour generator
	//static default_random_engine generator;
	//uniform_int_distribution<int> randomCol(0x00, 0xFF);

	// Random jitter generator
	static default_random_engine gen;
	uniform_real_distribution<float> randomFloat(0, 1);

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

	if (!quickRendered) {
		// Quick and dirty rendering
		// Staggered iteration over image pixels at 1/4 res with no AA
		static int i = 0;
		const int resDiv = 4;
		for (; i < n; i+=resDiv) {
			if (i % (_renderTexture->GetWidth()*resDiv) < _renderTexture->GetWidth()*(resDiv-1)) continue;

			float pixW = 1.0f / (float)(_renderTexture->GetWidth());
			float pixH = 1.0f / (float)(_renderTexture->GetHeight());
			float x = (i % (_renderTexture->GetPitch() / 4)) / (float)(_renderTexture->GetWidth());
			float y = (i / (_renderTexture->GetPitch() / 4)) / (float)(_renderTexture->GetHeight());
			//x = x * 2 - 1; y = y * 2 - 1;

			Camera *cam = _scene->GetCamera();
			glm::vec3 col = glm::vec3(0);

			// Directly render pixels, no sampling here
			Ray primary = cam->GeneratePrimary({ x, y });
			HitInfo primaryHit = _scene->Raycast(primary);
			col = Shade(primary, primaryHit);
			Uint32 c = MapCol(col);

			for (int u = 0; u < resDiv; u++) {
				for (int v = 0; v < resDiv; v++) {
					pixels[i + v + u*_renderTexture->GetWidth()] = c;
				}
			}

			// Break out of pixel-fill after 10ms
			if (i % 100 == 0) {
				auto current = Clock::now();
				auto time = chrono::duration_cast<chrono::milliseconds>(current - start);
				if (time.count() > 10) {
					//SDL_Log("%d\n", time.count());
					break;
				}
			}
		}
		if (i == n) {
			i = 0;
			quickRendered = true;
		}
	}
	else {
		// Full rendering
		// Staggered iteration over the image pixels (constant re-rendering)
		static int i = 0;
		for (; i < n; i++) {
			float pixW = 1.0f / (float)_renderTexture->GetWidth();
			float pixH = 1.0f / (float)_renderTexture->GetHeight();
			float x = (i % (_renderTexture->GetPitch() / 4)) / (float)_renderTexture->GetWidth();
			float y = (i / (_renderTexture->GetPitch() / 4)) / (float)_renderTexture->GetHeight();
			//x = x * 2 - 1; y = y * 2 - 1;

			Camera *cam = _scene->GetCamera();
			glm::vec3 col = glm::vec3(0);

			// Sub-pixel jittered multisampling
			int n = 2;
			for (int i = 0; i < n; i++) {		// Up the pixel
				for (int j = 0; j < n; j++) {	// Across the pixel
					float px = x - 0.5f*pixW + (j + randomFloat(gen))*(pixW / n);
					float py = y - 0.5f*pixH + (i + randomFloat(gen))*(pixH / n);
					Ray primary = cam->GeneratePrimary({ px, py });
					HitInfo primaryHit = _scene->Raycast(primary);
					col += Shade(primary, primaryHit);
				}
			}

			// MSAA - divide by no. of samples
			pixels[i] = MapCol(col / (float)(n*n));


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
		if (i == n) {
			i = 0;
			rendered = true;
		}
	}
	

	_renderTexture->Unlock();
}

glm::vec3 Raytracer::Shade(const Ray &ray, const HitInfo &hitInfo, int depth) {
	if (depth > 3) return{ 0, 0, 0 };

	if (!hitInfo.hit) {
		// Sky
		glm::vec3 col = { 0.7f, 0.7f, 1.0f };
		if (ray.dir.y < 0) {
			return 1.3f*col;
		}
		else {
			return glm::vec3(1.3f-ray.dir.y) * col;
		}
	}

	// Grab the material and lights
	Material *m = hitInfo.obj->mat;
	auto lights = &_scene->lights;

	// Reflection vector used throughout
	glm::vec3 reflect = glm::reflect(ray.dir, hitInfo.n);

	// General phong lighting
	glm::vec3 litCol(0.1f); // ambient should be replaced by GI sampling
	glm::vec3 specCol(0.0f);
	for (int i = 0; i < lights->size(); i++) {
		LightInfo li = lights->at(i)->CalcLightingInfo(hitInfo.p);

		// Generate the shadow ray
		Ray shadowRay = Ray(hitInfo.p + 0.01f * li.dir, li.dir);
		HitInfo shadowHit = _scene->Raycast(shadowRay);

		if (!shadowHit.hit || shadowHit.dist > li.shadowDist) {
			// Shading
			float base = glm::dot(li.dir, hitInfo.n);
			if (base < 0) continue;
			float spec = powf(glm::dot(li.dir, reflect), m->specPow);
			spec = (spec < 0) ? 0 : spec; // negative spec is bad spec

			litCol += glm::vec3(base)*li.col;
			specCol += glm::vec3(spec)*li.col;
		}
	}

	// Reflections
	glm::vec3 reflection = { 0, 0, 0 };

	if (m->reflectivity > 0.001f) {
		Ray reflectRay = Ray(hitInfo.p + 0.001f * reflect, reflect);
		HitInfo reflectHit = _scene->Raycast(reflectRay);
		reflection = Shade(reflectRay, reflectHit, depth + 1);
	}

	// Transparency / fresnel
	glm::vec3 background = { 0, 0, 0 };
	float nr = 1.0f / m->refractiveIndex;
	glm::vec3 refract = glm::refract(ray.dir, hitInfo.n, nr);

	float cosI = glm::dot(-ray.dir, hitInfo.n);
	float cosT = glm::dot(refract, -hitInfo.n);
	float rPar = (nr*cosI - cosT) / (nr*cosI + cosT);
	float rPer = (cosI - nr*cosT) / (cosI + nr*cosT);

	if (m->opacity < 0.999f) {
		Ray refractRay = Ray(hitInfo.p + 0.01f * refract, refract);
		HitInfo refractHit = _scene->Raycast(refractRay);

		glm::vec3 outRefract = glm::refract(refractRay.dir, -refractHit.n, m->refractiveIndex);
		Ray outRefractRay = Ray(refractHit.p + 0.01f * outRefract, outRefract);
		HitInfo outRefractHit = _scene->Raycast(outRefractRay);

		background = Shade(outRefractRay, outRefractHit, depth + 1) * (1.0f - m->opacity);
	}

	float kr = 0.5f * (rPar*rPar + rPer*rPer) * m->reflectivity;
	reflection *= kr;
	background *= (1.0f - kr);

	// Surface shading
	// Combine components according to shadowing
	glm::vec3 surf = { 0, 0, 0 };
	surf = litCol*m->diffuse*m->opacity*(1.0f-kr) + specCol*m->spec;

	return surf + background + reflection;
}

void Raytracer::RenderEnd() {
	_scene = nullptr;
	_camera = nullptr;
}