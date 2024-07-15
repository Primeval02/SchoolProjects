#include "RayTracing.h"

#include "BVH.h"

#include "imgui.h"
#include "UIHelpers.h"

void RTOBJMesh::update(const Transform2D& transform) {

	min = vec3(std::numeric_limits<float>::max());
	max = vec3(-std::numeric_limits<float>::max());
	mat4 M =
		glm::translate(transform.translation) *
		(glm::rotate(glm::radians(transform.rotation.z), vec3(0, 0, 1))
			* glm::rotate(glm::radians(transform.rotation.y), vec3(0, 1, 0))
			* glm::rotate(glm::radians(transform.rotation.x), vec3(1, 0, 0)))
		* glm::scale(transform.scale);
	for (int i = 0; i < vertices.size(); i++) {
		auto& vertex = vertices[i];
		const auto& bindVertex = bindVertices[i];
		vertex.position = M * vec4(bindVertex.position, 1);

		min = glm::min(min, vertex.position);
		max = glm::max(max, vertex.position);
	}

	center = (min + max) * 0.5f;
}

void RaySceneObject::update() {
	if (type == +RTShapeType::Plane) {
		if (changed) {
			R = mat4();

			if (glm::length(glm::vec3(transform.rotation)) > 0.f) {
				//R = glm::rotate(object.transform.rotation.w, vec3(object.transform.rotation));
				R = glm::rotate(glm::radians(transform.rotation.z), vec3(0, 0, 1))
					* glm::rotate(glm::radians(transform.rotation.y), vec3(0, 1, 0))
					* glm::rotate(glm::radians(transform.rotation.x), vec3(1, 0, 0));

				invR = glm::inverse(R);
			}


			vec3 N = R * vec4(0, 1, 0, 0);

			vec3 P = transform.translation;
			float d = -glm::dot(P, N);

			data = vec4(N, d);

			changed = false;
		}
	}
	if (type == +RTShapeType::Mesh) {
		if (changed) {
			if (RTOBJMesh* mesh = (RTOBJMesh*)pointer) {
				if (mesh->bvh) {
					mesh->bvh->build(transform, mesh->bvh->maxDepth);
				}
				else {
					mesh->update(transform);
				}
			}
			
			changed = false;
		}
	}
}

void RaySceneObject::hit(const Ray& ray, HitRecord& hr) {
	switch (type) {
	case RTShapeType::Plane:
		RayPlaneHit(*this, ray, hr);
		break;
	case RTShapeType::Sphere:
		RaySphereHit(*this, ray, hr);
		break;
	case RTShapeType::Box:
		RayBoxHit(*this, ray, hr);
		break;
	case RTShapeType::Mesh:
		if (RTOBJMesh* mesh = (RTOBJMesh*)pointer) {
			if (useBVH) {
				RayMeshHitBVH(*this, ray, hr);
			}
			else {
				RayMeshHit(*this, ray, hr);
			}
		}
		break;
	case RTShapeType::Tri:
		RayTriHit(*this, ray, hr);
		break;
	default:
		break;
	}
}

void RaySceneObject::renderUI() {
	ImGui::PushID((const void*)this);

	if (ImGui::CollapsingHeader(name.c_str())) {

		changed |= ImGui::Checkbox("Enabled", &enabled);
		changed |= renderEnumDropDown("Shape type", type);
		if (ImGui::CollapsingHeader("Transform")) {
			changed |= ImGui::InputFloat3("Translate", glm::value_ptr(transform.translation));
			changed |= ImGui::SliderFloat3("Translate sliders", glm::value_ptr(transform.translation), -10.0f, 10.0f);

			ImGui::Text("Rotation");
			changed |= ImGui::SliderFloat3("Euler angles", glm::value_ptr(transform.rotation), 0.0f, 360.0f);
			changed |= ImGui::InputFloat3("Scale", glm::value_ptr(transform.scale));

			if (type == +RTShapeType::Sphere) {
				changed |= ImGui::InputFloat("Radius", &data.w);
			}
		}
		if (ImGui::CollapsingHeader("Material")) {
			material.renderUI();
		}
		if (type == +RTShapeType::Mesh) {
			ImGui::Checkbox("Use BVH", &useBVH);
		}

		if (ImGui::Button("Delete")) {
			shouldDelete = true;
		}
	}

	if (type == +RTShapeType::Tri) {
		vec3* data = (vec3*)pointer;
		ImGui::InputFloat3("V1", (float*)&data[0]);
		ImGui::InputFloat3("V2", (float*)&data[1]);
		ImGui::InputFloat3("V3", (float*)&data[2]);
	}

	ImGui::PopID();
}

// Compute whether a ray hits a plane:
// ray p(t) = p0 + r*t
// plane p = ax + by + cz + d = 0
//		p*n + d = 0
//		(p0 + rt) . n + d = 0
//		p0 . n + tr . n + d = 0
//		tr . n = -(d + p0.n)
//		t = -(d + p0 . n) / (r . n)
bool RayPlaneHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
	vec3 N = vec3(object.data);
	float d = object.data.w;

	float numerator = -(d + glm::dot(ray.p0, N));
	float denominator = glm::dot(ray.R, N);
	//if (denominator > 0) {
	//	N = -N;
	//	numerator = -(-d + glm::dot(ray.p0, N));
	//	denominator = glm::dot(ray.R, N);
	//}
	if (denominator < 0.0f) {
		float t = numerator / denominator;

		// Intersection
		if (t >= 0.f) {

			if (object.transform.scale.x == 0 || object.transform.scale.z == 0) {
				if (t < hr.t) {
					hr.hit = true;
					hr.object = &object;
					hr.t = t;
					hr.position = ray.p(t);
					hr.normal = glm::normalize(N);
					return true;
				}
			}
			else {
				// See if point p is within the max width and height of the plane
				vec3 hitPosition = ray.p(t);

				// To see if p is within the range of N, we need to convert the hit position into the plane's coordinates.
				// Rotate the plane so it's facing straight up
				auto hitToCenter = hitPosition - object.transform.translation;
				vec3 hitPositionPlaneSpace = object.invR * vec4(hitToCenter, 1.0f);

				if (glm::abs(hitPositionPlaneSpace.x * 0.5f) < object.transform.scale.x &&
					glm::abs(hitPositionPlaneSpace.z * 0.5f) < object.transform.scale.z) {
					if (t < hr.t) {
						hr.hit = true;
						hr.object = &object;
						hr.t = t;
						hr.position = ray.p(t);
						hr.normal = glm::normalize(N);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool RayTriHit(const vec3& v1, const vec3& v2, const vec3& v3, const RaySceneObject& object,
	const Ray& ray, HitRecord& hr) {
	// Get plane of triangle
	vec3 a = v2 - v1;
	vec3 b = v3 - v1;

	vec3 N = glm::normalize(glm::cross(a, b));
	float d = -glm::dot(v1, N);

	float numerator = -(d + glm::dot(ray.p0, N));
	float denominator = glm::dot(ray.R, N);
	//if (denominator > 0) {
	//	N = -N;
	//	numerator = -(-d + glm::dot(ray.p0, N));
	//	denominator = glm::dot(ray.R, N);
	//}
	if (denominator < 0.0f) {
		float t = numerator / denominator;

		// Intersection with plane - now check to see if point is inside triangle
		if (t >= 0.f && t < hr.t) {
			//hr.t = t;
			//hr.position = ray.p(t);
			//hr.normal = N;
			//hr.object = &object;
			//hr.hit = true;
			//return;

			// Get intersection point
			vec3 P = ray.p(t);

			mat4 R = glm::toMat4(glm::rotation(N, vec3(0, 1, 0)));

			vec3 pLocal = P - v1;

			vec3 pTriangle = R * vec4(pLocal, 1);
			vec3 aTri = R * vec4(a, 1);
			vec3 bTri = R * vec4(b, 1);


			// Get barycentric coordinates of point with respect to triangle
			vec3 bary;

			mat2 R2;
			R2[0] = vec2(aTri.x, aTri.z);
			R2[1] = vec2(bTri.x, bTri.z);

			vec2 L = glm::inverse(R2) * vec2(pTriangle.x, pTriangle.z);

			bary = vec3(1.0 - L.x - L.y, L.x, L.y);

			// Check if barycentric conditions are valid: all between 0 and 1, and sum to 1

			bool inTriangle = true;
			float sum = 0.0f;
			for (int i = 0; i < 3; i++) {
				if (bary[i] < 0.0f || bary[i] > 1.0f) {
					inTriangle = false;
					break;
				}
				sum += bary[i];
			}

			if (inTriangle && glm::abs(sum - 1) > 1e-4f) {
				inTriangle = false;
			}

			if (inTriangle) {
				hr.t = t;
				hr.hit = true;
				hr.position = P;
				hr.normal = N;
				hr.object = &object;
				hr.bary = bary;
				return true;
			}
		}
	}

	return false;
}

bool RayTriHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
	vec3* points = (vec3*)object.pointer;
	const vec3& v1 = points[0];
	const vec3& v2 = points[1];
	const vec3& v3 = points[2];
	return RayTriHit(v1, v2, v3, object, ray, hr);
}

bool RaySphereHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {

	float r = object.data.w;
	if (r <= 0) return false;

	vec3 center = object.transform.translation;

	vec3 p0c = ray.p0 - center;

	float a = glm::dot(ray.R, ray.R);
	float b = 2.0f * glm::dot(ray.R, p0c);
	float c = glm::dot(p0c, p0c) - (r * r);

	float det = glm::sqrt(b * b - 4 * a * c);
	if (det > 0) {
		float t1 = (-b + det) / (2 * a);
		float t2 = (-b - det) / (2 * a);

		if (t1 >= 0 && t2 >= 0) {
			float mint = glm::min(t1, t2);

			if (mint < hr.t) {
				hr.t = mint;
				hr.object = &object;
				hr.hit = true;
				hr.position = ray.p(mint);
				hr.normal = glm::normalize(hr.position - center);
				return true;
			}
		}
		else if (t1 >= 0) {
			if (t1 < hr.t) {
				hr.t = t1;
				hr.object = &object;
				hr.hit = true;
				hr.position = ray.p(t1);
				hr.normal = glm::normalize(hr.position - center);
				return true;
			}
		}
		else {
			if (t2 < hr.t) {
				hr.t = t2;
				hr.object = &object;
				hr.hit = true;
				hr.position = ray.p(t2);
				hr.normal = glm::normalize(hr.position - center);
				return true;
			}
		}
	}
	else if (det == 0) {
		float t = (-b) / (2 * a);

		if (t < hr.t) {
			hr.t = t;
			hr.object = &object;
			hr.hit = true;
			hr.position = ray.p(t);
			hr.normal = glm::normalize(hr.position - center);
			return true;
		}
	}

	return false;
}



// Compute whether a ray hits a box:
// determine the 6 planes of the box
// pick the 3 planes that the ray has a chance of hitting
// see if the ray hits any of those 3

bool RayBoxHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
	static constexpr vec3 unitDirections[6] = {
		vec3(-1, 0, 0), vec3(1, 0, 0),
		vec3(0, -1, 0), vec3(0, 1, 0),
		vec3(0, 0, -1), vec3(0, 0, 1),
	};

	static constexpr vec3 unitX[6] = {
		vec3(0, 0, 1), vec3(0, 0, -1),
		vec3(1, 0, 0), vec3(1, 0, 0),
		vec3(-1, 0, 0), vec3(1, 0, 0),
	};

	static constexpr vec3 unitZ[6] = {
		vec3(0, 1, 0), vec3(0, 1, 0),
		vec3(0, 0, 1), vec3(0, 0, -1),
		vec3(0, 1, 0), vec3(0, 1, 0)
	};

	static constexpr ivec2 scaleIndices[6] = {
		ivec2(2, 1), ivec2(2, 1),
		ivec2(0, 2), ivec2(0, 2),
		ivec2(0, 1), ivec2(0, 1)
	};

	mat4 S = glm::scale(object.transform.scale);

	mat4 T = glm::translate(object.transform.translation);

	mat4 R;
	if (glm::length2(vec3(object.transform.rotation)) > 0) {
		R = glm::rotate(glm::radians(object.transform.rotation.z), vec3(0, 0, 1))
			* glm::rotate(glm::radians(object.transform.rotation.y), vec3(0, 1, 0))
			* glm::rotate(glm::radians(object.transform.rotation.x), vec3(1, 0, 0));
	}

	mat4 M = T * R * S;

	for (int i = 0; i < 6; i++) {
		const vec3& unitDir = unitDirections[i];
		const vec3& x = unitX[i];
		const vec3& z = unitZ[i];

		vec3 N = glm::normalize(R * vec4(unitDir, 0));

		if (glm::dot(N, ray.R) >= 0) continue;

		vec3 P = M * vec4(unitDir, 1);

		vec3 X = R * vec4(x, 0);
		vec3 Z = R * vec4(z, 0);

		X = glm::normalize(X);
		Z = glm::normalize(Z);

		RaySceneObject rso;
		rso.type = RTShapeType::Plane;
		rso.transform.translation = P;
		rso.data = vec4(N, -glm::dot(P, N));
		rso.R = glm::identity<mat4>();
		rso.R[0] = vec4(X, 0);
		rso.R[1] = vec4(N, 0);
		rso.R[2] = vec4(Z, 0);
		rso.invR = glm::transpose(rso.R);

		rso.transform.scale.x = object.transform.scale[scaleIndices[i].x] * 0.5f;
		rso.transform.scale.y = 0;
		rso.transform.scale.z = object.transform.scale[scaleIndices[i].y] * 0.5f;
		if (RayPlaneHit(rso, ray, hr) && hr.object == &rso) {
			hr.object = &object;
			return true;
		}
	}

	return false;
}

bool RayMeshHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
	RTOBJMesh* mesh = (RTOBJMesh*)object.pointer;

	if (!mesh) return false;

	for (int i = 0; i < mesh->indices.size(); i += 3) {
		const auto& v1 = mesh->vertices[i].position;
		const auto& v2 = mesh->vertices[i + 1].position;
		const auto& v3 = mesh->vertices[i + 2].position;

		bool hitThisTri = RayTriHit(v1, v2, v3, object, ray, hr);

		if (hitThisTri) {
			vec3 normal = vec3(0);
			for (int j = 0; j < 3; j++) {
				normal += hr.bary[j] * mesh->vertices[i + j].normal;
			}
			hr.normal = glm::normalize(normal);
			return true;
		}
	}

	return false;
}

bool RayMeshHitBVH(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
	RTOBJMesh* mesh = (RTOBJMesh*)object.pointer;

	if (!mesh) return false;

	return mesh->bvh->hit(ray, hr);
}