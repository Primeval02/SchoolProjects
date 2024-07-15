//Name: Bradley Tate
#include "Lab06.h"
#include <iostream>
#include "Application.h"
#include "Texture.h"
#include "Primitives.h"
#include "Framebuffer.h"

#include "imgui.h"

#include "Input.h"
#include "UIHelpers.h"
#include "StringUtil.h"
#include "Renderer.h"

// Same as celestial bodies from lab 05. Used to hold the 
// color and model matrix for individual objects. Does not store mesh data.
std::vector<SceneObject> sceneObjects;

struct Lab06Camera {
	vec3 cameraPosition = vec3(0, 0, 5);
	vec3 cameraLookat = vec3(0, 0, 0);
	vec3 cameraUp = vec3(0, 1, 0);
	bool cameraOrtho = false;
	float cameraFOVY = 60.0f;
	vec2 nearFar = vec2(1, 1000);
};

Lab06Camera camera;

struct Mesh {
	std::vector<GLfloat> positions;
	std::vector<GLuint> indices;

	vec4 color;
};

Mesh triangleMesh;
Mesh sphereMesh;

// Vertex array object
GLuint VAO = 0;

// Vertez buffer objects
GLuint VBO = 0; //mesh positions
GLuint IBO = 0;	//mesh indices

// Loads mesh data onto the GPU as buffer objects.
// Uses the following calls:
// glGenVertexArrays
// glBindVertexArray
// glGenBuffers
// glBindBuffer
// glBufferData
// glVertexAttribPointer
// glEnableVertexAttribArray
void initMeshDataonGPU() {

	/*for (int i = 0; i < Sphere::instance.positions.size(); i++) {
		triangleMesh.positions.pushback(i);
	}*/

	// Add 3 vertices to mesh
	// Vertex 0: (-1, -1, -0)
	triangleMesh.positions.push_back(-1);
	triangleMesh.positions.push_back(-1);
	triangleMesh.positions.push_back(0);

	// Vertex 1: (1, -1, 0)
	triangleMesh.positions.push_back(1);
	triangleMesh.positions.push_back(-1);
	triangleMesh.positions.push_back(0);

	// Vertex 2: (1, 1, 0)
	triangleMesh.positions.push_back(1);
	triangleMesh.positions.push_back(1);
	triangleMesh.positions.push_back(0);

	// Vertex 3: (-1, 1, 0)	
	triangleMesh.positions.push_back(-1);
	triangleMesh.positions.push_back(1);
	triangleMesh.positions.push_back(0);

	// Add 3 indices to triangleMesh
	triangleMesh.indices.push_back(0);
	triangleMesh.indices.push_back(1);
	triangleMesh.indices.push_back(2);

	// 3 Verticies for triangle 2
	triangleMesh.indices.push_back(2);
	triangleMesh.indices.push_back(3);
	triangleMesh.indices.push_back(0);

	triangleMesh.color = vec4(1.0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate VBO for position data
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, triangleMesh.positions.size() * sizeof(GLfloat), triangleMesh.positions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);


	// Generate IBO for indeces data
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleMesh.indices.size() * sizeof(GLuint), triangleMesh.indices.data(), GL_STATIC_DRAW);
}


GLuint vertexShader = 0, fragmentShader = 0;
GLuint shaderProgram = 0;


// Compiles and links shader program from source.
// Uses the following calls:
// glCreateShader
// glShaderSource
// glCompileShader
// glGetShaderiv
// glGetShaderInfoLog
// glCreateProgram
// glAttachShader
// glLinkProgram
// glGetProgramiv
// glGetProgramInfoLog
void initShader() {
	// The R"TOKEN()TOKEN"; notation just lets us specify a "raw" string literal that supports
	// multiple lines, " characters, and so on without escaping characters. Great for shader sources.
	std::string vertexShaderText = Renderer::shaderVersion + "\n\n" + R"VERTEXSHADER(

uniform mat4 mvp;

in vec3 vertexPosition;

void main() {
	gl_Position = mvp * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
}

)VERTEXSHADER";

	std::string fragmentShaderText = Renderer::shaderVersion + "\n\n" + R"FRAGMENTSHADER(

uniform vec4 triColor;

out vec4 FragColor;

void main() {
	FragColor = triColor;
}

)FRAGMENTSHADER";

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char * vertexShaderTextPtr = vertexShaderText.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderTextPtr, 0);
	glCompileShader(vertexShader);
	GLint compileStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		char logInfo[512];
		glGetShaderInfoLog(vertexShader, 512, 0, logInfo);
		log("Vertex shader failed to compile:\n{0}", logInfo);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fragmentShaderTextPtr = fragmentShaderText.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderTextPtr, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		char logInfo[512];
		glGetShaderInfoLog(fragmentShader, 512, 0, logInfo);
		log("Fragment shader failed to compile:\n{0}", logInfo);
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		char logInfo[512];
		glGetProgramInfoLog(shaderProgram, 512, 0, logInfo);
		log("Shader failed to link:\n{0}", logInfo);
	}

}

// Initializes the lab data using the above function calls and global variables. 
void Lab06::init() {
	initMeshDataonGPU();
	initShader();

	initialized = true;
}

// Renders objects using the framebuffer.
// Uses the following calls:
// glUseProgram
// glBindVertexArray
// matrix setup calls from glm: glm::perspective for projection, glm::lookAt for view, T*R*S for model.
// glGetUniformLocation to get the location of a uniform variable in a shader
// glUniform* to set a uniform's value in a shader
// glDrawElements to draw contents of buffer objects
void Lab06::render(s_ptr<Framebuffer> framebuffer) {
	if (!initialized) init();

	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	// Set the triangle MVP using uniforms
	mat4 view = glm::lookAt(camera.cameraPosition, camera.cameraLookat, camera.cameraUp);
	float aspectRatio = (float)framebuffer->width / (float)framebuffer->height;
	mat4 projection = glm::perspective(glm::radians(camera.cameraFOVY), aspectRatio, camera.nearFar.x, camera.nearFar.y);



	for (const auto& sceneObject : sceneObjects) {
		// TODO: render each scene object
		mat4 model = sceneObject.transform.getMatrixGLM();

		mat4 mvp = projection * view * model;

		GLint mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		// Set the triangle color using uniforms
		GLint triColorLocation = glGetUniformLocation(shaderProgram, "triColor");
		glUniform4fv(triColorLocation, 1, glm::value_ptr(sceneObject.color));

		glDrawElements(GL_TRIANGLES, triangleMesh.indices.size(), GL_UNSIGNED_INT, 0);

	}
}

void Lab06::renderUI() {

	if (ImGui::Button("Initialize Lab 06")) {
		init();
	}

	ImGui::ColorEdit4("Triangle color", glm::value_ptr(triangleMesh.color));

	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::SliderFloat3("Position", glm::value_ptr(camera.cameraPosition), -100, 100);
		ImGui::InputFloat3("Position manual", glm::value_ptr(camera.cameraPosition));
		ImGui::SliderFloat3("Lookat", glm::value_ptr(camera.cameraLookat), -100, 100);
		ImGui::InputFloat3("Lookat manual", glm::value_ptr(camera.cameraLookat));
		ImGui::InputFloat3("Up", glm::value_ptr(camera.cameraUp));
		ImGui::SliderFloat("FOVY", &camera.cameraFOVY, 0, 360);
		ImGui::InputFloat2("Near-far", glm::value_ptr(camera.nearFar));

		if (ImGui::Button(camera.cameraOrtho ? "Orthographic" : "Perspective")) {
			camera.cameraOrtho = !camera.cameraOrtho;
		}
	}

	if (ImGui::CollapsingHeader("Scene objects")) {
		IMDENT;

		static int numberToAdd = 10;

		if (ImGui::Button("Add new")) {
			SceneObject newObject;
			newObject.name = fmt::format("Object {0}", sceneObjects.size() + 1);
			sceneObjects.push_back(newObject);
		}
		ImGui::SetNextItemWidth(200);
		ImGui::InputInt("Number to add", &numberToAdd);
		ImGui::SameLine();
		std::string addRandomLabel = fmt::format("Add {0}", numberToAdd);
		if (ImGui::Button(addRandomLabel.c_str())) {
			for (int i = 0; i < glm::max(numberToAdd, 0); i++) {
				SceneObject newObject;
				newObject.name = fmt::format("Object {0}", sceneObjects.size() + 1);

				newObject.transform.translation = glm::linearRand(vec3(-200), vec3(200));
				newObject.transform.scale = vec3(glm::linearRand(0.1f, 8.f));
				newObject.color = glm::linearRand(vec3(0.1f), vec3(1.f));
				sceneObjects.push_back(newObject);
			}
		}

		if (sceneObjects.size() > 0) {
			if (ImGui::Button("Clear all")) {
				sceneObjects.clear();
			}

			int counter = 1;

			ImGui::Text("Number of objects: %lu", sceneObjects.size());

			auto soToDelete = sceneObjects.end();
			for (auto it = sceneObjects.begin(); it != sceneObjects.end() && counter++ < 100; ++it) {
				auto& cb = *it;
				IMDENT;
				cb.renderUI();
				IMDONT;
				if (cb.shouldDelete) {
					soToDelete = it;
				}
			}

			if (soToDelete != sceneObjects.end()) {
				sceneObjects.erase(soToDelete);
			}
		}
		
		IMDONT;
	}
}
