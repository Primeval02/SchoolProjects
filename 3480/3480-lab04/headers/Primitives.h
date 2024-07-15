#pragma once

#include "globals.h"
#include "imgui.h"

// Some functions that will be common to assignments (or from old assignments that get promoted to the global codebase)
MAKE_ENUM(LineRenderMode, int, Implicit, Parametric)
MAKE_ENUM(ParametricLineMode, int, Samples, Midpoint, Brenesam, DDA)
MAKE_ENUM(TriangleRenderMode, int, Outline, Parametric, BoundingBox)

// Vertex: represents a single point in space. Has attributes that a rasterizer
// or ray-tracer could interpolate for rendering purposes. 
struct Vertex {
	vec3 position = vec3(0.f);
	vec3 color = vec3(1.f);

	void renderUI(const char* label = "Vertex") {
		ImGui::PushID((const void*)this);
		if (ImGui::CollapsingHeader(label)) {
			IMDENT;
			ImGui::InputFloat2("Position", glm::value_ptr(position));
			ImGui::ColorEdit3("Color", glm::value_ptr(color));
			IMDONT;
		}
		ImGui::PopID();
	}
};

using Pixel = Vertex;

// Line: a primitive between 2 vertices. 
struct Line {
	Vertex p0;
	Vertex p1;
	vec3 color = vec3(1.f);
	float thickness = 1.0f;
	bool enabled = true;
	bool shouldDelete = false;

	// Returns the distance between a point P and the line between p0 and p1.
	// The parameterized distance is assigned to the reference parameter t so 
	// the calling function can use the value for interpolation. 
	float dist(vec2 p, float& t) const {
		// h: the vector representing the hypotenuse of the triangle between p0, p, and the closest point on the line to p
		vec2 h = p - vec2(p0.position);
		// r: the ray of the line
		vec2 r = p1.position - p0.position;

		// The dot product between h and r is the squared length of h's projection onto r.
		// Dividing it by r gives us a parameterized value t representing how "far along"
		// point p is along the line segment. For p to be "in between" p0 and p1, then 
		// t must be in [0, 1].
		t = glm::dot(h, r) / glm::length2(r);

		// But t could be past either end of the line, so we do distance checks to p0 and p1, 
		// the closest points on the line to p in either case.
		if (t < 0) {
			return glm::length(p - vec2(p0.position));
		}
		else if (t > 1) {
			return glm::length(p - vec2(p1.position));
		}

		// pp is the closest point on the line to p
		vec2 pp = vec2(p0.position) + t * r;

		// And so we can take the length of p - pp as the distance. 
		return glm::length(p - pp);
	}

	void renderUI() {
		ImGui::Checkbox("Enabled", &enabled);
		p0.renderUI("P0");
		p1.renderUI("P1");
		ImGui::InputFloat("Thickness", &thickness);
		if (ImGui::Button("Delete")) {
			shouldDelete = true;
		}
	}
};

struct Triangle {
	Vertex vertices[3];

	vec3 color = vec3(1.f);
	bool enabled = true;
	bool shouldDelete = false;

	Vertex& p0() { return vertices[0]; }
	Vertex& p1() { return vertices[1]; }
	Vertex& p2() { return vertices[2]; }

	Triangle() { }
	Triangle(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& col) {
		vertices[0] = { v1, vec3(1.f) };
		vertices[1] = { v2, vec3(1.f) };
		vertices[2] = { v3, vec3(1.f) };
		color = col;
	}

	// Computes the barycentric coordinates for point p such that
	// p = bary.x * p0 + bary.y * p1 + bary.z * p2;
	// Works in 2D.

	// Uses the triangle positions as non-orthogonal basis for point p.
	// In matrix form, (p - p0) = (p1 - p0, p2 - p0) * b
	vec3 barycentric(vec3 p) {
		vec2 P = vec2(p) - vec2(p0().position);
		mat2 R;
		R[0] = vec2(p1().position - p0().position);
		R[1] = vec2(p2().position - p0().position);

		vec2 L = glm::inverse(R) * P;

		return vec3(1.0f - L.x - L.y, L.x, L.y);
	}

	Vertex computeFromBarycentric(vec3 bary) {
		Vertex result = { vec3(0.f), vec3(0.f) };

		for (int i = 0; i < 3; i++) {
			result.position += bary[i] * vertices[i].position;
			result.color += bary[i] * vertices[i].color;
		}

		return result;
	}

	bool baryInTriangle(vec3 bary) {
		float sum = 0.0f;
		for (int i = 0; i < 3; i++) {
			if (bary[i] < 0.0f || bary[i] > 1.0f) return false;
			sum += bary[i];
		}

		if (sum < 0.99f || sum > 1.01f) return false;

		return true;
	}

	void renderUI() {
		ImGui::Checkbox("Enabled", &enabled);
		vertices[0].renderUI("P0");
		vertices[1].renderUI("P1");
		vertices[2].renderUI("P2");
		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		if (ImGui::Button("Delete")) {
			shouldDelete = true;
		}
	}
};

struct Icosphere {
	vec3 positions[12] = {
		{  0,		 -1,		 0		  },
		{  0.723600, -0.447215,	 0.525720 },
		{ -0.276385, -0.447215,	 0.850640 },
		{ -0.894425, -0.447215,	 0.000000 },
		{ -0.276385, -0.447215,	-0.850640 },
		{  0.723600, -0.447215,	-0.525720 },
		{  0.276385,  0.447215,	 0.850640 },
		{ -0.723600,  0.447215,	 0.525720 },
		{ -0.723600,  0.447215,	-0.525720 },
		{  0.276385,  0.447215,	-0.850640 },
		{  0.894425,  0.447215,	 0.000000 },
		{  0,		  1,		 0		  }
	};

	vec3 colors[12] = {
		vec3(47, 79, 79) / 255.0f,
		vec3(139, 69, 19) / 255.0f,
		vec3(34, 139, 34) / 255.0f,
		vec3(0, 0, 139) / 255.0f,
		vec3(255, 0, 0) / 255.0f,
		vec3(255, 215, 0) / 255.0f,
		vec3(127, 255, 0) / 255.0f,
		vec3(0, 255, 255) / 255.0f,
		vec3(255, 0, 255) / 255.0f,
		vec3(100, 149, 237) / 255.0f,
		vec3(255, 105, 180) / 255.0f,
		vec3(255, 228, 196) / 255.0f,
	};

	ivec3 indices[20] = {
		{  0,  1,  2  },
		{  1,  0,  5  },
		{  0,  2,  3  },
		{  0,  3,  4  },
		{  0,  4,  5  },
		{  1,  5,  10 },
		{  2,  1,  6  },
		{  3,  2,  7  },
		{  4,  3,  8  },
		{  5,  4,  9  },
		{  1, 10,  6  },
		{  2,  6,  7  },
		{  3,  7,  8  },
		{  4,  8,  9  },
		{  5,  9,  10 },
		{  6, 10,  11 },
		{  7,  6,  11 },
		{  8,  7,  11 },
		{  9,  8,  11 },
		{ 10,  9,  11 }
	};
};

struct Circle {
	Vertex center;
	float radius;
	float tolerance;

	float dist(vec2 coordinate) const {
		vec2 diff = coordinate - vec2(center.position);

		return glm::length(diff);
	}

	void renderUI() {
		ImGui::InputFloat2("Center", glm::value_ptr(center.position));
		ImGui::InputFloat("Radius", &radius);
		ImGui::InputFloat("Tolerance", &tolerance);
		ImGui::ColorEdit3("Color", glm::value_ptr(center.color));
	}
};

MAKE_ENUM(RotationAxis, int, X, Y, Z);

struct Transform2D {
	// Translation: offsets to X and Y position
	vec3 translation = vec3(0.f);
	// Rotation: rotates point around center of rotation (X, Y, Z) by angle (W)
	vec4 rotation = vec4(0.f);
	// Scale: resizes based on width (X) and height (Y) and depth (Z)
	vec3 scale = vec3(1.f);

	bool autoSpin = false;
	RotationAxis axis = RotationAxis::Z;

	bool autoFall = false;

	// Computes a 4x4 matrix that can transform 2D and 3D points by the translation, rotation, and scale listed above
	mat4 getMatrix();

	void renderUI();
};

void renderLineParametric(const Line& line, float* pixels, int stride, int width, int height);
void renderLineImplicit(const Line& line, float* pixels, int stride, int width, int height);
void renderCircle(const Circle &circle, float* pixels, int stride, int width, int height);

void renderTriangleOutline(Triangle& tri, float* pixels, int stride, int width, int height);
void renderTriangleParametric(Triangle& tri, float* pixels, int stride, int width, int height);
void renderTriangleBoundingBox(Triangle& tri, float* pixels, int stride, int width, int height);

void renderIcosphere(Icosphere& ico, float* pixels, int stride, int width, int height, bool useColors = true);