// Name: Bradley Tate

#include "Lab12.h"

#include "Application.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "GPU.h"
#include "Input.h"
#include "Primitives.h"
#include "Renderer.h"
#include "StringUtil.h"
#include "Texture.h"
#include "UIHelpers.h"

#include "imgui.h"
#include "ImGuiFileDialog.h"

// TODO: update the vertex and fragment shaders to use Blinn-Phong shading.
void Raymarcher::init() {

	const char* RaymarcherVertexShaderSrc = R"VERTEXSHADER(
__VERSION__

#define Z 0.0

// Built-in vertices for the two fullscreen triangles
const vec4 vertices[6] = vec4[6](
	vec4(-1, -1, Z, 1), 
	vec4(1, -1, Z, 1), 
	vec4(1, 1, Z, 1),
	vec4(1, 1, Z, 1), 
	vec4(-1, 1, Z, 1), 
	vec4(-1, -1, Z, 1)
);

void main() {
	gl_Position = vertices[gl_VertexID];
}

)VERTEXSHADER";

	const char* RaymarcherFragmentShaderSrc = R"FRAGMENTSHADER(
__VERSION__

uniform SceneObject {
	mat4 objects[256];
} scene;

uniform vec3 cameraPosition;
uniform int numObjects;
uniform vec3 lightDirection;
uniform mat4 inv_projection;
uniform mat4 inv_view;
uniform mat4 view;
uniform vec2 resolution;

out vec4 FragColor;

struct HitRecord {
	bool hit;
	float t;
	vec3 position;
	vec3 normal;
	vec3 bary;
	vec4 color;
};

HitRecord HR() {
	HitRecord hr;
	hr.hit = false;
	hr.t = 1e38;
	hr.position = vec3(0.);
	hr.normal = vec3(0.);
	hr.bary = vec3(0.);
	hr.color = vec4(0.);
	return hr;
}

// +: p outside sphere
// 0: p on sphere
// -: p inside sphere
float sdSphere(vec3 p, vec3 center, float radius) {
	return length(p - center) - radius;
}

// plane equation: dot(P, n) + d = 0
float sdPlane(vec3 p , vec4 plane) {
	return dot(p, plane.xyz) + plane.w;
}

// box equation
float sdBox(vec3 p, vec3 b) {
	vec3 q = abs(p) - b;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

void main() {
	// ss in range of [0, 1]
	vec2 ss = gl_FragCoord.xy / resolution;

	// convert ss to ndc: [0, 1] to [-1, 1]
	vec2 ndc = ss * 2.0 - vec2(1.0);

	// convert with iverse projection
	vec3 preview = (inv_projection * vec4(ndc.x, ndc.y, 0.0, 1.0)).xyz;
	vec3 world = (inv_view * vec4(preview.xyz, 1.0)).xyz;

	vec3 ro = cameraPosition;
	vec3 rd = normalize(world - cameraPosition);

	vec4 sphere = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 plane = vec4(0.0, 1.0, 0.0, 0.0);

	vec3 boxCenter = vec3(-2.0, 1.0, 0.0);
	vec3 boxSize = vec3(0.5, 2.0, 3.0);

	HitRecord hr = HR();

	//bool cameraUnderPlane = sdPlane(cameraPosition, plane) < 0;

	for (float t = 0.0; t < 16.0; t += 0.03125) {
		vec3 p = ro + t * rd;

		for (int i = 0; i < numObjects; i++) {
			mat4 object = scene.objects[i];

			//If the object is disabled, skip over it
			bool enabled = object[2].x > 0;

			if (!enabled) continue;

			int type = int(object[2].y);
			
			// type == 0 is a sphere
			if (type == 0) {
				vec3 center = object[0].xyz;
				float radius = object[0].w;
				float d = sdSphere(p, center, radius);
				if (d <= .0 && t < hr.t) {
					hr.hit = true;
					hr.t = t;
					hr.position = p;
					hr.normal = normalize(p - center);
					hr.color = object[3];
					break;
				}
			}

			// type == 1 is a plane
			else if (type == 1) {
				bool cameraAbovePlane = sdPlane(cameraPosition, object[0]) >= 0;
				if (cameraAbovePlane) {
					float d = sdPlane(p, object[0]);
					if (d <= 0. && t < hr.t) {
						hr.hit = true;
						hr.t = t;
						hr.position = p;
						hr.normal = plane.xyz;
						hr.color = object[3];
						break;
					}
				}
			}

			// type == 2 is a box
			else if (type == 2) {
				vec3 boxCenter = object[0].xyz;
				vec3 boxSize = object[1].xyz;
				float d = sdBox(p - boxCenter, boxSize);
				if (d <= 0. && t < hr.t) {
					hr.hit = true;
					hr.t = t;
					hr.position = p;
					hr.normal = -rd;
					hr.color = object[3];
					break;
				}
			}
		}

		/*
		float d1 = sdSphere(p, sphere.xyz, sphere.w);

		if (d1 <= 0. && t < hr.t) {
			hr.hit = true;
			hr.t = t;
			hr.position = p;
			hr.normal = normalize(p - sphere.xyz);
			hr.color = vec4(1.0);
			break;
		}

		if (!cameraUnderPlane) {
			float d2 = sdPlane(p, plane);
			if (d2 <= 0. && t < hr.t) {
				hr.hit = true;
				hr.t = t;
				hr.position = p;
				hr.normal = plane.xyz;
				hr.color = vec4(1.0, 0.0, 0.0, 1.0);
				break;
			}
		}

		float d3 = sdBox(p - boxCenter, boxSize);
		if (d3 <= 0. && t < hr.t) {
			hr.hit = true;
			hr.t = t;
			hr.position = p;
			hr.normal = -rd;
			hr.color = vec4(0.0, 0.0, 1.0, 1.0);
			break;
		}
		*/
	}
	
	if (hr.hit) {
		float intensity = max(0.0, dot(hr.normal, -lightDirection));
		FragColor = vec4(intensity * hr.color);
	}
	else {
		FragColor = vec4(rd, 1.0);
	}
}

)FRAGMENTSHADER";

	if (!shader.init(RaymarcherVertexShaderSrc, RaymarcherFragmentShaderSrc)) {
		exit(1);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	addSceneObject(RaymarchObjectType::Sphere, vec3(0, 0, -10), vec3(1.0f), vec4(0.1f, 0.6f, 0.9f, 1.0f));

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * MAX_OBJECTS, (const void*)sceneObjects.data(), GL_DYNAMIC_DRAW);

	GLuint blockLocation = glGetUniformBlockIndex(shader.program, "SceneObject");
	glUniformBlockBinding(shader.program, blockLocation, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, sizeof(mat4) * MAX_OBJECTS);


	camera.distRange = vec2(camera.distRange.x, 10.0);


	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	initialized = true;
}

// Renders to the "screen" texture that has been passed in as a parameter
void Raymarcher::render(s_ptr<Framebuffer> framebuffer) {
	if (!initialized) init();

	double deltaTime = Application::get().deltaTime;
	double timeSinceStart = Application::get().timeSinceStart;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(shader.program);

	camera.update(float(framebuffer->width), float(framebuffer->height));

	if (light.autoOrbit) {
		vec3 forward = vec3(0, 0, -1);

		if (glm::abs(glm::dot(forward, light.orbitAxis)) > 0.99f) {
			forward = vec3(1, 0, 0);
		}

		mat4 orbit = glm::rotate((float)timeSinceStart, light.orbitAxis);

		light.lightDirection = orbit * vec4(forward, 0);
	}


	GLint lightDirLocation = glGetUniformLocation(shader.program, "lightDirection");
	glUniform3fv(lightDirLocation, 1, glm::value_ptr(light.lightDirection));

	GLint cpLoc = glGetUniformLocation(shader.program, "cameraPosition");
	if (cpLoc != -1) {
		glUniform3fv(cpLoc, 1, glm::value_ptr(camera.cameraPosition));
	}

	GLint vLoc = glGetUniformLocation(shader.program, "view");
	if (vLoc != -1) {
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
	}

	GLint ipLoc = glGetUniformLocation(shader.program, "inv_projection");
	if (ipLoc != -1) {
		mat4 inv_projection = glm::inverse(camera.projection);
		glUniformMatrix4fv(ipLoc, 1, GL_FALSE, glm::value_ptr(inv_projection));
	}

	GLint ivLoc = glGetUniformLocation(shader.program, "inv_view");
	if (ivLoc != -1) {
		mat4 inv_view = glm::inverse(camera.view);
		glUniformMatrix4fv(ivLoc, 1, GL_FALSE, glm::value_ptr(inv_view));
	}

	GLint rLoc = glGetUniformLocation(shader.program, "resolution");
	if (rLoc != -1) {
		glUniform2f(rLoc, framebuffer->width, framebuffer->height);
	}

	GLint noLoc = glGetUniformLocation(shader.program, "numObjects");
	if (noLoc != -1) {
		glUniform1i(noLoc, numObjects);
	}


	glBindVertexArray(VAO);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	if (objectsChanged) {
		glBufferSubData(GL_UNIFORM_BUFFER, 0, numObjects * sizeof(mat4), (const void*)sceneObjects.data());
		objectsChanged = false;
	}
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

bool renderUIRaymarchObject(int index, mat4 & object, bool & shouldDelete) {
	bool changed = false;
	std::string label = fmt::format("Object {0}", index);
	if (ImGui::CollapsingHeader(label.c_str())) {
		bool enabled = object[2].x > 0;
		if (ImGui::Checkbox("Enabled", &enabled)) {
			object[2].x = (float)enabled;
			changed = true;
		}
		int type = object[2].y;
		RaymarchObjectType rot = RaymarchObjectType::_from_integral(type);
		if (renderEnumDropDown("Type", rot)) {
			object[2].y = rot._value;
			changed = true;
		}
		switch (type) {
			case +RaymarchObjectType::Sphere:
				changed |= ImGui::InputFloat3("Position", glm::value_ptr(object[0]));
				changed |= ImGui::InputFloat("Radius", &object[0].w);
				break;
			case +RaymarchObjectType::Plane:
				changed |= ImGui::InputFloat3("Position", glm::value_ptr(object[1]));
				changed |= ImGui::InputFloat3("Normal", glm::value_ptr(object[0]));
				if (ImGui::Button("Make unit vector")) {
					object[0] = vec4(glm::normalize(vec3(object[0])), 0.f);
					changed = true;
				}

				if (changed) {
					object[0].w = -glm::dot(vec3(object[0]), vec3(object[1]));
				}
				break;
			case +RaymarchObjectType::Box:
				changed |= ImGui::InputFloat3("Position", glm::value_ptr(object[0]));
				changed |= ImGui::InputFloat3("Size", glm::value_ptr(object[1]));
				break;
			default:
				break;
		}
		changed |= ImGui::ColorEdit4("Color", glm::value_ptr(object[3]));
	}

	return changed;
}

void Raymarcher::renderUI() {

	light.renderUI();
	camera.renderUI();

	if (ImGui::CollapsingHeader("Raymarch objects")) {
		IMDENT;

		static int numberToAdd = 10;

		if (ImGui::Button("Add new")) {
			addSceneObject();
		}

		if (sceneObjects.size() > 0) {
			if (ImGui::Button("Clear all")) {
				numObjects = 0;
			}

			int counter = 0;

			ImGui::Text("Number of objects: %lu", sceneObjects.size());

			for (int i = 0; i < numObjects && i < MAX_OBJECTS; i++) {
				auto& cb = sceneObjects[i];
				IMDENT;
				bool shouldDelete = false;
				ImGui::PushID((const void*)&cb);
				objectsChanged |= renderUIRaymarchObject(i, cb, shouldDelete);
				ImGui::PopID();
				IMDONT;
				if (shouldDelete) {
					//soToDelete = it;
				}
			}
		}

		IMDONT;
	}
}
