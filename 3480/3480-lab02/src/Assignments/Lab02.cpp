// Name: Bradley Tate
//
// Highest OpenGL for laptop: 3.3


#include "Lab02.h"
#include "Texture.h"
#include "imgui.h"
#include "UIHelpers.h"

struct Pixel {
	int x;
	int y;
	vec3 color; 
};

struct line {
	vec2 p0;
	vec2 p1;
	vec3 color;
};

std::vector<Pixel> savedPixels;
std::vector<line> savedLines;

int numSteps = 100;

void renderLines(float * pixels, int width, int height, int stride) {
	for(auto& line : savedLines) {
		vec2 r = line.p1 - line.p0;
		float rLength = glm::length(r);
		float dt = 1.0f / rLength;

		for (float t = 0.0f; t <= 1.0f; t += dt) {
			vec2 p = line.p0 + (r * t);
			int x = glm::floor(p.x);
			int y = glm::floor(p.y);

			size_t offset = ((y * width) + x) * stride;
			auto pixel = pixels + offset;

			//method 1
			pixel[0] = line.color.x;
			pixel[1] = line.color.y;
			pixel[2] = line.color.z;

			//method 2
			vec3* pixelColor = (vec3*)pixel;
			*pixelColor = line.color;
		}
	}
}
// Renders to the "screen" texture that has been passed in as a parameter
void Lab02::render(s_ptr<Texture> screen) {

	auto mem = screen->memory;
	auto value = mem->value;
	auto pixels = (float*)mem->value;

	size_t numPixels = (size_t)screen->resolution.x * screen->resolution.y;

	for (size_t y = 0; y < screen->resolution.y; y++) {
		for (size_t x = 0; x < screen->resolution.x; x++) {

			size_t offset = ((y * screen->resolution.x) + x) * mem->stride;

			auto pixel = pixels + offset;

			pixel[0] = 0.0f;
			pixel[1] = 0.0f;
			pixel[2] = 0.0f;
			
		}
	}

	for (auto& pixel : savedPixels) {

		if (pixel.x < 0 || pixel.y < 0 || pixel.x >= screen->resolution.x || pixel.y >= screen->resolution.y) continue;		

		size_t offset = ((pixel.y * screen->resolution.x) + pixel.x) * mem->stride;

		auto pixelLocation = pixels + offset;

		pixelLocation[0] = pixel.color.x;
		pixelLocation[1] = pixel.color.y;
		pixelLocation[2] = pixel.color.z;

	}

	renderLines(pixels, screen->resolution.x, screen->resolution.y, mem->stride);	

	glBindTexture(GL_TEXTURE_2D, screen->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen->resolution.x, screen->resolution.y, 0, GL_RGBA, GL_FLOAT, (const void*)pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Lab02::renderUI() {
	static vec2 p0 = { 0, 0 };
	static vec2 p1 = { 0, 0 };
	static vec4 color = { 1, 1, 1, 1 };

	ImGui::InputFloat2("P0", glm::value_ptr(p0));
	ImGui::InputFloat2("P1", glm::value_ptr(p1));
	ImGui::ColorEdit4("Color", glm::value_ptr(color));
	ImGui::InputInt("Num steps", &numSteps);

	if (ImGui::Button("Add line")) {
		line newLine;
		newLine.p0 = p0;
		newLine.p1 = p1;
		newLine.color = color;

		savedLines.push_back(newLine);
	}

	if (ImGui::Button("Add pixel")) {
		Pixel newPixel;
		newPixel.x = p0.x;
		newPixel.y = p0.y;
		newPixel.color = vec3(color.x, color.y, color.z);

		savedPixels.push_back(newPixel);
	}

	if (ImGui::CollapsingHeader("Lines")) {
		int lineCounter = 0;

		for (auto& line : savedLines) {
			std::string lineLabel = fmt::format("Line {0}", lineCounter++);

			if (ImGui::CollapsingHeader(lineLabel.c_str())) {
				IMDENT;
				ImGui::InputFloat2("P0", glm::value_ptr(line.p0));
				ImGui::InputFloat2("P1", glm::value_ptr(line.p1));
				ImGui::ColorEdit3("Color", glm::value_ptr(line.color));
				IMDONT;
			}
		}	
	}

	
}
