#pragma once

#include "Assignment.h"
#include "RayTracing.h"
#include "GPU.h"

class TextureMemory;

class RayTracer : public Assignment {
public:

	Camera camera;
	std::vector<Lighting> lights;

	s_ptr_vector<RaySceneObject> sceneObjects;

	RTColorMode colorMode = RTColorMode::Default;

	bool computeShadow = true;
	bool renderEveryFrame = true;
	bool renderNextFrame = false;

	RayTracer() : Assignment("Ray tracer", false) { }
	virtual ~RayTracer() { }

	virtual void init();
	virtual void render(s_ptr<Texture> screen);

	void raytraceScene(const Ray& ray, HitRecord& hr);
	vec4 computeHitColor(const HitRecord& hr, const Ray& ray, const Lighting& light, int depth);

	s_ptr<RaySceneObject> addSceneObject() {
		s_ptr<RaySceneObject> newObject = std::make_shared<RaySceneObject>();
		newObject->name = fmt::format("Object {0}", sceneObjects.size() + 1);
		sceneObjects.push_back(newObject);
		return newObject;
	}

	s_ptr<RaySceneObject> addPlane(const vec3& position = vec3(0.f), const vec3& normal = vec3(0.f, 1.f, 0.f), const vec2 & scale = vec2(0.f)) {
		s_ptr<RaySceneObject> newObject = addSceneObject();
		newObject->type = RTShapeType::Plane;
		newObject->transform.translation = position;
		newObject->transform.rotation = vec4(glm::degrees(glm::eulerAngles(glm::rotation(vec3(0, 1, 0), normal))), 0.f);

		if (scale.x > 0.f && scale.y > 0.f) {
			newObject->transform.scale = vec3(scale.x, 0.f, scale.y);
		}
		else {
			newObject->transform.scale = vec3(0.f);
		}
		return newObject;
	}

	s_ptr<RaySceneObject> addSphere(const vec3& position = vec3(0.f), float radius = 1.0f) {
		s_ptr<RaySceneObject> newObject = addSceneObject();
		newObject->type = RTShapeType::Sphere;
		newObject->transform.translation = position;
		newObject->data.w = radius;
		return newObject;
	}

	s_ptr<RaySceneObject> addBox(const vec3& position = vec3(0.f), const vec3 & rotation = vec3(0.f), const vec3 & scale = vec3(1.f)) {
		s_ptr<RaySceneObject> newObject = addSceneObject();
		newObject->type = RTShapeType::Box;
		newObject->transform.translation = position;
		newObject->transform.rotation = vec4(rotation, 0);
		newObject->transform.scale = scale;
		return newObject;
	}

	virtual void renderUI();
};
