// CMPS 3480: Lab 11
#include "Lab11.h"

#include "Application.h"

#include "Texture.h"

#include "UIHelpers.h"

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

	//#pragma omp parallel for
	for (int x = 0; x < (int)width; x++) {
		for (int y = 0; y < height; y++) {

			// Step 1: convert from screen-space to NDC
			vec2 ss(x + 0.5f, y + 0.5f);
			ss.x = ss.x / (width - 1);
			ss.y = ss.y / (height - 1);
			// ss is now in range of [0, 0] to [1, 1]
			vec2 ndc = (ss * 2.0f) - vec2(1);

			vec3 preview = invProjection * vec4(ndc.x, ndc.y, 0, 1);
			vec3 world = invView * vec4(preview.x, preview.y, preview.z, 1);

			Ray ray = { camera.cameraPosition, glm::normalize(world - camera.cameraPosition) };

			size_t offset = ((y * width) + x) * stride;

			vec4* pixel = (vec4*)(pixels + offset);

			vec4 color = vec4(0);

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