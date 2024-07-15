#pragma once

#include "Assignment.h"

#include "GPU.h"

MAKE_ENUM(RaymarchObjectType, int, Sphere, Plane, Box)

class Raymarcher : public Assignment {
public:
	GLuint VAO = 0;

	Shader shader;
	Camera camera;
	Lighting light;

	// Uniform storage for raymarch objects
	GLuint UBO = 0;
	static constexpr GLuint MAX_OBJECTS = 256;
	bool objectsChanged = false;
	int numObjects = 0;
	std::array<mat4, MAX_OBJECTS> sceneObjects;

	Raymarcher() : Assignment("Ray marcher", true) { }
	virtual ~Raymarcher() { }

	virtual void init();
	virtual void render(s_ptr<Framebuffer> framebuffer);
	virtual void renderUI();

	void addSceneObject(RaymarchObjectType type = RaymarchObjectType::Sphere, vec3 position = vec3(0.f), vec3 scale = vec3(1.f), vec4 color = vec4(1.0f)) {
		mat4 newObject;
		// columns 1 and 2 allow you to store up to 8 float values to represent this object.
		// Sphere: col1.xyz is position, col1.w is radius
		// Plane: col1.xyz is the normal, col1.w is the d value. col2.xyz is the position (center) used to compute d
		// Box: col2.xyz is position, col2.xyz is size in x, y, and z
		switch (type) {
			case +RaymarchObjectType::Sphere: {
				newObject[0] = vec4(position, scale.x);
				break;
			}
			case +RaymarchObjectType::Plane: {
				vec3 normal = glm::normalize(scale);
				newObject[0] = vec4(normal, -glm::dot(position, normal));
				newObject[1] = vec4(position, 0);
				break;
			}
			case +RaymarchObjectType::Box: {
				newObject[0] = vec4(position, 0.0f);
				newObject[1] = vec4(scale, 0.0f);
				break;
			}
			default:
				break;
		}
		// column 3: x is enabled (0 or 1), y is type (RayMarchObject)
		newObject[2] = vec4(1.0f, type._to_integral(), 0.0f, 0.0f);
		// column 4 is color
		newObject[3] = color;
		sceneObjects[numObjects] = newObject;

		numObjects = (numObjects + 1) % MAX_OBJECTS;
	}
};
