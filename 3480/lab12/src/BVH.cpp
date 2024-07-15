// Not a good implementation - see https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/

#include "BVH.h"
#include "Lab11.h"
#include "imgui.h"

void BVH::build(const Transform2D& transform, int depth) {

	if (!mesh) return;
	maxDepth = glm::max(0, depth);

	mesh->update(transform);
	
	numNodes = 0;
	root = nullptr;
	root = std::make_shared<BVHNode>(this, mesh->min, mesh->max, nullptr);

	log("BVH created with {0} nodes\n", numNodes);
}

bool BVH::hit(const Ray& ray, HitRecord& hr) {
	if (!mesh) return false;

	return root->hit(ray, hr);
}

BVHNode::BVHNode(BVH* _bvh, const vec3& _min, const vec3& _max, BVHNode* _parent)
	: bvh(_bvh), min(_min), max(_max), parent(_parent) {

	bvh->numNodes++;

	vec3 size = max - min;
	// Translate and scale only for now since this is axis-aligned.
	/*mat4 minBox = glm::translate(bvh->mesh->min) * glm::scale(size);
	min = minBox * vec4(0, 0, 0, 1);
	max = minBox * vec4(1, 1, 1, 1);*/

	vec3 center = (min + max) * 0.5f;
	object.transform.translation = center;
	object.transform.scale = size * 0.5f;

	if (parent == nullptr) {
		depth = 0;
	}
	// A parent decided our min/max for us
	else {
		depth = parent->depth + 1;
	}

	if (depth < bvh->maxDepth) {
		vec3 halfSize = (max - min) * 0.5f;
		// Create 8 children
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					vec3 childMin = min + vec3(x * halfSize.x, y * halfSize.y, z * halfSize.z);
					vec3 childMax = childMin + halfSize;
					auto child = std::make_shared<BVHNode>(bvh, childMin, childMax, this);
					child->object.material.Kd = vec3(x, y, z);

					if (child->depth < bvh->maxDepth) {
						children.push_back(child);
					}
					else if (!child->triangles.empty()) {
						children.push_back(child);
					}
					else {
						log("Node at depth {0} doesn't have any tris - throwing it away!\n", child->depth);
					}
					
				}
			}
		}

		if (children.size() > 1) {
			for (int i = 0; i < children.size(); i++) {
				auto ci = children[i];
				for (int j = i + 1; j < children.size(); j++) {
					auto cj = children[j];

					if (ci->min == cj->min && ci->max == cj->max) {
						log("Children {0} and {1} have same bounds - oughta merge em\n", i, j);

						ci->triangles.insert(ci->triangles.end(), cj->triangles.begin(), cj->triangles.end());
						ci->vertices.insert(ci->vertices.end(), cj->vertices.begin(), cj->vertices.end());

						std::sort(ci->triangles.begin(), ci->triangles.end());
						std::sort(ci->vertices.begin(), ci->vertices.end());
						ci->triangles.erase(std::unique(ci->triangles.begin(), ci->triangles.end()), ci->triangles.end());
						ci->vertices.erase(std::unique(ci->vertices.begin(), ci->vertices.end()), ci->vertices.end());

						children.erase(children.begin() + j);
						j = j - 1;
					}

				}
			}
		}
		
	}
	// Only store triangles in the leaf nodes. 
	else {
		vec3 triMin(std::numeric_limits<float>::max());
		vec3 triMax(-std::numeric_limits<float>::max());
		bool sizesChanged = false;

		vec3 triVertex[3];
		for (int i = 0; i < bvh->mesh->indices.size(); i += 3) {
			int triIndex = i / 3;

			bool triInBox = false;
			bool triAllInBox = true;
			for (int j = 0; j < 3; j++) {
				triVertex[j] = bvh->mesh->vertices[i + j].position;

				bool within = glm::all(glm::greaterThanEqual(triVertex[j], min))
					&& glm::all(glm::lessThanEqual(triVertex[j], max));

				if (within) {
					vertices.push_back(i + j);
				}

				triInBox |= within;

				triAllInBox &= within;
			}

			if (triInBox) {
				sizesChanged = true;
				for (int j = 0; j < 3; j++) {
					triMin = glm::min(triVertex[j], triMin);
					triMax = glm::max(triVertex[j], triMax);
				}
			}

			if (triInBox) {
				triangles.push_back(triIndex);
			}
		}

		if (sizesChanged) {
			min = triMin;
			max = triMax;

			vec3 triCenter = (triMin + triMax) * 0.5f;
			vec3 triSize = triMax - triMin;

			object.transform.translation = triCenter;
			object.transform.scale = triSize * 0.5f;
		}
	}
}

bool BVHNode::hitTriangles(const Ray& ray, HitRecord& hr) {

	bool hit = false;
	//HitRecord hc;
	for (auto& tri : triangles) {
		int vstart = tri * 3;
		const auto& v1 = bvh->mesh->vertices[vstart].position;
		const auto& v2 = bvh->mesh->vertices[vstart+1].position;
		const auto& v3 = bvh->mesh->vertices[vstart+2].position;

		bool hitThisTri = RayTriHit(v1, v2, v3, object, ray, hr);

		if (hitThisTri) {
			vec3 normal(0);
			for (int j = 0; j < 3; j++) {
				normal += hr.bary[j] * bvh->mesh->vertices[vstart + j].normal;
			}
			hr.normal = glm::normalize(normal);
			hr.object = &bvh->root->object;
		}
		hit |= hitThisTri;
	}

	//if (hc.hit && hc.t < hr.t) {
	//	hr = hc;
	//	hr.object = &bvh->root->object;
	//}



	return hit;
}

bool BVHNode::hit(const Ray& ray, HitRecord& hr) {

	HitRecord nodeHit;

	if (RayBoxHit(object, ray, nodeHit)) {
		if (children.empty()) {

			hitTriangles(ray, hr);
		}
		else {
			
			//bool hit = false;
			//s_ptr<BVHNode> childHit = nullptr;
			//HitRecord hc;
			for (auto& child : children) {
				child->hit(ray, hr);
				/*if (RayBoxHit(child->object, ray, hc)) {
					if (hc.t < hr.t) {
						childHit = child;
						hr = hc;
					}
				}*/
			}

			//if (childHit) {
			//	return childHit->hit(ray, hr);
			//	//return true;
			//}
		}
	}

	return false;
	
}

void BVH::renderUI() {
	ImGui::InputInt("Triangles per box", &trisPerBox);
}