// CMPS 3480: Lab 11
#include "Lab11.h"

#include "Application.h"

#include "Texture.h"

#include "UIHelpers.h"

MAKE_ENUM(RTColorMode, int, Default, Position, Normal);

RTColorMode colorMode = RTColorMode::Default;

void RaySceneObject::renderUI() {
	ImGui::PushID((const void*)this);

	if (ImGui::CollapsingHeader(name.c_str())) {

		changed |= ImGui::Checkbox("Enabled", &enabled);
		changed |= renderEnumDropDown("Shape type", type);
		if (ImGui::CollapsingHeader("Transform")) {
			changed |= ImGui::InputFloat3("Translate", glm::value_ptr(transform.translation));
			changed |= ImGui::SliderFloat3("Translate sliders", glm::value_ptr(transform.translation), -10.0f, 10.0f);

			if (type == +RTShapeType::Plane || type == +RTShapeType::Box) {
				ImGui::Text("Rotation");
				changed |= ImGui::SliderFloat3("Euler angles", glm::value_ptr(transform.rotation), 0.0f, 360.0f);
				changed |= ImGui::InputFloat3("Scale", glm::value_ptr(transform.scale));
			}
			else if (type == +RTShapeType::Sphere) {
				changed |= ImGui::InputFloat("Radius", &data.w);
			}

		}
		if (ImGui::CollapsingHeader("Material")) {
			material.renderUI();
		}
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
void RayPlaneHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
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
					}
				}
			}
		}
	}
}

void RaySphereHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {

	float r = object.data.w;
	if (r <= 0) return;

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
			}
		}
		else if (t1 >= 0) {
			if (t1 < hr.t) {
				hr.t = t1;
				hr.object = &object;
				hr.hit = true;
				hr.position = ray.p(t1);
				hr.normal = glm::normalize(hr.position - center);
			}
		}
		else {
			if (t2 < hr.t) {
				hr.t = t2;
				hr.object = &object;
				hr.hit = true;
				hr.position = ray.p(t2);
				hr.normal = glm::normalize(hr.position - center);
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
		}

	}
}



// Compute whether a ray hits a box:
// determine the 6 planes of the box
// pick the 3 planes that the ray has a chance of hitting
// see if the ray hits any of those 3

void RayBoxHit(const RaySceneObject& object, const Ray& ray, HitRecord& hr) {
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

	for(int i = 0; i < 6; i++) {
		const vec3& unitDir = unitDirections[i];
		const vec3& x = unitX[i];
		const vec3& z = unitZ[i];

		vec3 N = glm::normalize(R * vec4(unitDir, 0));

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
		RayPlaneHit(rso, ray, hr);

		if (hr.hit && hr.object == &rso) {
			hr.object = &object;
		}
	}
}

void RaySceneObject::hit(const Ray& ray, HitRecord& hr) {
	switch (type) {
		case RTShapeType::Plane:
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
			RayPlaneHit(*this, ray, hr);
			break;
		case RTShapeType::Sphere:
			RaySphereHit(*this, ray, hr);
			break;
		case RTShapeType::Box:
			RayBoxHit(*this, ray, hr);
			break;
		default:
			break;
	}
}

void RayTracer::init() {

	auto floor = addPlane(vec3(0.f, -2.0f, 0.f), vec3(0, 1, 0), vec2(1.f, 1.f));
	floor->material.Kd = vec3(0.6f);

	auto ceil = addPlane(vec3(0.f, 2.0f, 0.f), vec3(0, -1, 0), vec2(1.f, 1.f));
	ceil->material.Kd = vec3(0.6f);

	auto left = addPlane(vec3(-2.0f, 0.f, 0.f), vec3(1, 0, 0), vec2(1.f));
	left->material.Kd = vec3(1.0f, 0.2f, 0.2f);
	left->material.reflective = 0.9f;

	auto right = addPlane(vec3(2.0f, 0.f, 0.f), vec3(-1, 0, 0), vec2(1.f));
	right->material.Kd = vec3(1.0f, 0.2f, 0.2f);

	auto back = addPlane(vec3(0.0f, 0.f, -2.f), vec3(0, 0, 1), vec2(1.f));
	back->material.Kd = vec3(0.1f, 0.2f, 0.8f);

	auto sphere = addSphere(vec3(0, -1, 0));
	sphere->material.Kd = vec3(0.f, 1.f, 1.f);
	//sphere->material.reflective = 0.9f;

	auto sphere2 = addSphere(vec3(0, 1, 0), 0.75f);
	sphere2->material.Kd = vec3(1.f, 0.f, 1.f);
	//sphere2->material.reflective = 0.1f;

	auto box1 = addBox(vec3(-1, -1, 1), vec3(0, 30, 0), vec3(0.25f, 1, 0.25f));
	box1->material.Kd = vec3(1.f);

	auto box2 = addBox(vec3(1, -1, 1), vec3(0, -30, 0), vec3(0.25f, 1, 0.25f));
	box2->material.Kd = vec3(1.f);

	Lighting light;

	light.Ia = 0.15f;

	light.point = true;
	light.position = vec3(1.0f, 1.0f, 1.0f);
	light.intensity = 1.0f;
	light.radius = 8.0f;

	lights.push_back(light);

	initialized = true;
}

void RayTracer::raytraceScene(const Ray& ray, HitRecord& hr) {
	for (auto& o : sceneObjects) {
		if (o->enabled) {
			o->hit(ray, hr);
		}
	}
}

int maxDepth = 10;

vec4 RayTracer::computeHitColor(const HitRecord& hr, const Ray& ray, const Lighting& light, int depth) {
	if (depth > maxDepth) return vec4(0);

	vec3 lightDirection = -glm::normalize(light.lightDirection);
	float intensity = 1.0f;

	vec3 lightPosition = light.position;

	if (light.point) {
		lightPosition = lightPosition + glm::sphericalRand(1.0f) * light.softness;
		vec3 lightDiff = lightPosition - hr.position;
		float lightDist = glm::length(lightDiff);
		lightDirection = lightDiff * (1.0f / lightDist);

		if (lightDist < light.radius) {
			intensity = 1.0f - (lightDist / light.radius);
			// Inverse square law
			intensity *= intensity;

			intensity = light.intensity * intensity;
		}
		else {
			intensity = 0.f;
		}
	}

	vec3 ambient = light.Ia * light.Ka;
	vec3 diffuse = glm::max(0.0f, glm::dot(lightDirection, hr.normal)) * light.Kd * hr.object->material.Kd * intensity;


	vec3 viewDirection = glm::normalize(camera.cameraPosition - hr.position);
	vec3 halfway = glm::normalize(lightDirection + viewDirection);
	vec3 specular = glm::pow(glm::max(0.0f, glm::dot(halfway, hr.normal)), light.shininess) * light.Ks;


	if (computeShadow && light.point) {
		// Need to cast a ray from here to the light source and see if we hit anything
		Ray lightRay;

		lightRay.p0 = hr.position + (1e-4f * hr.normal);
		vec3 lightRayDiff = lightPosition - lightRay.p0;
		float lightRayDist = glm::length(lightRayDiff);
		if (lightRayDist > 0) {
			lightRay.R = lightRayDiff * (1.0f / lightRayDist);
			HitRecord lightHit;
			lightHit.t = lightRayDist;
			raytraceScene(lightRay, lightHit);

			// We hit something trying to get to the light
			if (lightHit.hit && lightHit.t >= 0 && lightHit.t < lightRayDist) {
				diffuse = vec3(0.f);
				specular = vec3(0.f);
			}
		}
	}

	vec3 color = (ambient + diffuse * intensity + specular) * hr.object->material.Kd;

	vec4 hitColor = vec4(color, 1.0);

	return hitColor;
}

// Renders objects using the framebuffer
void RayTracer::render(s_ptr<Texture> screen) {
	if (!initialized) init();

	float width = screen->resolution.x;
	float height = screen->resolution.y;
	auto mem = screen->memory;
	auto pixels = (float*)mem->value;

	auto sr = Application::get().getRenderer<SoftwareRenderer>();

	// Update the camera to use the window's dimensions, not the framebuffer. This ensures that whatever
	// resolution we render at, it will account for the destination's aspect ratio. 
	camera.update(sr->resolution.x, sr->resolution.y);
	//camera.update(width, height);

	if (!renderEveryFrame && !renderNextFrame) {
		glBindTexture(GL_TEXTURE_2D, screen->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen->resolution.x, screen->resolution.y, 0, GL_RGBA, GL_FLOAT, (const void*)pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}



	double deltaTime = Application::get().deltaTime;
	double timeSinceStart = Application::get().timeSinceStart;

	int stride = mem->stride;

	size_t maxSize = width * height * stride;

	for (auto& light : lights) {
		if (light.autoOrbit) {
			vec3 forward = vec3(0, 0, -1);

			if (glm::abs(glm::dot(forward, light.orbitAxis)) > 0.99f) {
				forward = vec3(1, 0, 0);
			}

			mat4 orbit = glm::rotate((float)timeSinceStart, light.orbitAxis);

			light.lightDirection = orbit * vec4(forward, 0);
		}
	}

	mat4 projection = camera.projection;
	mat4 view = camera.view;

	mat4 invProjection = glm::inverse(projection);
	mat4 invView = glm::inverse(view);

	vec2 dp = vec2(1.0f / (width - 1.0f), 1.0f / (height - 1.0f));

	vec2 halfdp = dp * 0.5f;

	int numSamples = 50;

	//#pragma omp parallel for
	for (int x = 0; x < (int)width; x++) {
		for (int y = 0; y < height; y++) {

			// Declare out of loop
			vec4 color = vec4(0);
			size_t offset = ((y * width) + x) * stride;
			vec4* pixel = (vec4*)(pixels + offset);
	

			// Sample size loop, generate offsets 
			for (int i = 0; i < numSamples; i++) {
				float xOffset = glm::linearRand(-0.5f, 0.5f);
				float yOffset = glm::linearRand(-0.5f, 0.5f);

				// Step 1: convert from screen-space to NDC
				vec2 ss(x + xOffset, y + yOffset);
				ss.x = ss.x / (width - 1);
				ss.y = ss.y / (height - 1);
				// ss is now in range of [0, 0] to [1, 1]
				vec2 ndc = (ss * 2.0f) - vec2(1);

				vec3 preview = invProjection * vec4(ndc.x, ndc.y, 0, 1);
				vec3 world = invView * vec4(preview.x, preview.y, preview.z, 1);

				Ray ray = { camera.cameraPosition, glm::normalize(world - camera.cameraPosition) };

				//size_t offset = ((y * width) + x) * stride;

				vec4* pixel = (vec4*)(pixels + offset);

				HitRecord hr;

				//vec2 ndcr = ndc;

				//vec3 previewr = invProjection * vec4(ndcr.x, ndcr.y, 0, 1);
				//vec3 worldr = invView * vec4(previewr.x, previewr.y, previewr.z, 1);
				//Ray rayr = { camera.cameraPosition, glm::normalize(worldr - camera.cameraPosition) };

				raytraceScene(ray, hr);

				if (hr.hit) {
					if (colorMode == +RTColorMode::Default) {

						for (auto& light : lights) {
							color += computeHitColor(hr, ray, light, 0);
						}
					}
					// These can be useful for debugging - they set the pixel color to the position or the normal
					// seen at the intersection point. 
					else if (colorMode == +RTColorMode::Position) {
						color += vec4(hr.position, 1.0f);
					}
					else if (colorMode == +RTColorMode::Normal) {
						vec3 n = (hr.normal + vec3(1.f)) * 0.5f;
						color += vec4(n, 1.0f);
					}

				}
				else {
					color += vec4(ray.R, 1.0f);
				}
			}

			// Average the color over all samples
			color /= numSamples;

			*pixel = color;
		}
	}


	glBindTexture(GL_TEXTURE_2D, screen->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen->resolution.x, screen->resolution.y, 0, GL_RGBA, GL_FLOAT, (const void*)pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	renderNextFrame = false;
}

void RayTracer::renderUI() {
	ImGui::Checkbox("Render every frame", &renderEveryFrame);
	if (!renderEveryFrame) {
		if (ImGui::Button("Render next frame")) {
			renderNextFrame = true;
		}
	}

	ImGui::Text("Color mode");
	ImGui::SameLine();
	renderEnumButton(colorMode);
	ImGui::Checkbox("Shadows", &computeShadow);
	ImGui::InputInt("Max depth", &maxDepth);

	camera.renderUI();

	if (ImGui::CollapsingHeader("Lights")) {
		if (ImGui::Button("Add")) {
			Lighting newLight;
			lights.push_back(newLight);
		}
		IMDENT;
		int lightID = 0;
		for (auto& light : lights) {
			std::string lightLabel = fmt::format("Light {0}", ++lightID);
			if (ImGui::CollapsingHeader(lightLabel.c_str())) {
				ImGui::PushID((const void*)&light);
				IMDENT;
				light.renderUI(false);
				ImGui::PopID();
			}
		}
		IMDONT;
	}


	if (ImGui::CollapsingHeader("Objects")) {
		if (ImGui::Button("Add object")) {
			addSceneObject();
		}
		for (auto& o : sceneObjects) {
			o->renderUI();
		}
	}
}
