#pragma once

#include "globals.h"
#include "Primitives.h"

#include "imgui.h"

MAKE_ENUM(RTShapeType, int, Plane, Tri, Quad, Sphere, Box, Mesh)
MAKE_ENUM(RTRenderSpeed, int, AllAtOnce, Partial, Progressive)
MAKE_ENUM(RTColorMode, int, Default, Position, Normal);

struct Ray {
	vec3 p0;
	vec3 R;

	vec3 p(float t) const {
		return p0 + t * R;
	}
};

struct RaySceneObject;

struct HitRecord {
	bool hit = false;
	float t = std::numeric_limits<float>::max();
	vec3 position;
	vec3 normal;
	vec3 bary;
	const RaySceneObject* object = nullptr;
};

struct Material {
	float reflective = 0.0f;
	float diffuseIndex = 0.0f;
	vec3 Kd = vec3(1.f);

	void renderUI() {
		ImGui::SliderFloat("Reflectivity", &reflective, 0, 1);
		ImGui::SliderFloat("Diffuse index", &diffuseIndex, 0, 1);
		ImGui::ColorEdit3("Diffuse color", glm::value_ptr(Kd));
	}
};

struct RTOBJMeshVertex {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
};

class BVH;

struct RTOBJMesh {
	std::vector<RTOBJMeshVertex> bindVertices;
	std::vector<RTOBJMeshVertex> vertices;
	std::vector<unsigned int> indices;

	vec3 min = vec3(std::numeric_limits<float>::max());
	vec3 max = vec3(-std::numeric_limits<float>::max());
	vec3 center = vec3(0);

	s_ptr<BVH> bvh;

	void update(const Transform2D& transform);


	bool isValid() {
		return !vertices.empty() && !indices.empty();
	}

	static RTOBJMesh import(const char* objFile);

	static RTOBJMesh getSphere();
};

struct RaySceneObject {
	bool enabled = true;
	bool changed = true;
	RTShapeType type = RTShapeType::Plane;
	std::string name = "object";
	Material material;
	Transform2D transform;

	// Contains mesh data if this object is a triangle mesh.
	// Contains 3 vertices if this object is a triangle.
	void* pointer = nullptr;

	// If mesh, whether to use BVH before raycasting
	bool useBVH = false;

	// Cached values to speed up raytracing
	mat4 R;		// Rotation matrix
	mat4 invR;	// Inverse rotation matrix

	// For planes: xyz is normal, w is d
	// For spheres: xyz is center, w is radius
	vec4 data;

	bool shouldDelete = false;

	virtual void update();

	virtual void hit(const Ray& ray, HitRecord& hr);

	virtual void renderUI();
};


bool RayPlaneHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RayTriHit(const vec3& v1, const vec3& v2, const vec3& v3, const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RayTriHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RaySphereHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RayBoxHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RayMeshHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr);

bool RayMeshHitBVH(const RaySceneObject& object, const Ray& ray, HitRecord& hr);