// Name: Bradley Tate

#include "Lab07.h"

#include "Application.h"
#include "Camera.h"
#include "Texture.h"
#include "Primitives.h"
#include "Framebuffer.h"

#include "imgui.h"

#include "Input.h"
#include "UIHelpers.h"
#include "StringUtil.h"
#include "Renderer.h"

// The two mesh types need different vertex array and buffer objects, but they can
// use the same index buffer object. Your job will be to implement the cube indices
// inside this struct's init function.
struct IndexBuffer {
	std::vector<GLuint> indices;
	GLuint IBO = 0;

	// This is a helper function that can be called to add 3 indices
	// as a triangle in the cube mesh. 
	void addTriangle(GLuint v0, GLuint v1, GLuint v2) {
		indices.push_back(v0);
		indices.push_back(v1);
		indices.push_back(v2);
	}

	void init() {
		// Mesh vertices are generated as 
		// 0: -1, -1, -1		// left		bottom	back
		// 1: -1, -1, +1		// left		bottom	front
		// 2: -1, +1, -1		// left		top		back
		// 3: -1, +1, +1		// left		top		front
		// 4: +1, -1, -1		// right	bottom	back
		// 5: +1, -1, +1		// right	bottom	front
		// 6: +1, +1, -1		// right	top		back
		// 7: +1, +1, +1		// right	top		front

		// TODO: populate the indices 
		// 6 sides to a box, 2 triangles each: 12 triangles
		// 12 triangles, 3 indices each: 36 indices

		// Front triangles of box:
		addTriangle(1, 5, 7);
		addTriangle(7, 3, 1);

		// Add two triangles for each remaining side: back, left, right, top, bottom

		// Back triangles:
		addTriangle(4, 0, 2);
		addTriangle(2, 6, 4);

		// Left triangles:
		addTriangle(0, 1, 3);
		addTriangle(3, 2, 0);

		// Right triangles:
		addTriangle(5, 4, 6);
		addTriangle(6, 7, 5);

		// Top triangles:
		addTriangle(3, 7, 6);
		addTriangle(6, 2, 3);

		// Bottom triangles:
		addTriangle(0, 4, 5);
		addTriangle(5, 1, 0);

		// Leave this code alone
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}
};

struct TriangleMeshSOA {
	std::vector<GLfloat> positions;
	std::vector<GLfloat> colors;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint CBO = 0;

	TriangleMeshSOA() {

		// Generates vertex positions and colors using nested for loop. Leave this code alone.
		for (int x = -1; x <= 1; x += 2) {
			for (int y = -1; y <= 1; y += 2) {
				for (int z = -1; z <= 1; z += 2) {
					vec3 position(x, y, z);
					vec3 color = (position + vec3(1)) * 0.5f;

					log("SOA Adding vertex {0}: {1}\n", positions.size(), glm::to_string(position));

					positions.push_back(position.x);
					positions.push_back(position.y);
					positions.push_back(position.z);

					colors.push_back(color.x);
					colors.push_back(color.y);
					colors.push_back(color.z);
				}
			}
		}
	}

	// Initializes the GPU data for the SAO triangle mesh. Already in working order.
	void init(GLuint IBO, GLuint shaderProgram) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);

		// Now that we have multiple vertex attributes, it's less safe to rely on hardcoded 0 or 1 locations. 
		// Instead, look up the attribute's location for the shader program. 
		GLint positionLoc = glGetAttribLocation(shaderProgram, "vPos");
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionLoc);

		glGenBuffers(1, &CBO);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

		GLint colorLoc = glGetAttribLocation(shaderProgram, "vCol");
		glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colorLoc);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBindVertexArray(0);
	}
};

struct MeshVertex {
	vec3 position;
	vec3 color;
};

struct TriangleMeshAOS {
	std::vector<MeshVertex> vertices;

	GLuint VAO = 0;
	GLuint VBO = 0;

	// Generates vertex positions and colors using nested for loop. Leave this code alone.
	TriangleMeshAOS () {
		for (int x = -1; x <= 1; x += 2) {
			for (int y = -1; y <= 1; y += 2) {
				for (int z = -1; z <= 1; z += 2) {
					vec3 pos(x, y, z);
					MeshVertex mv;
					mv.position = pos;
					mv.color = (pos + vec3(1)) * 0.5f;

					log("AOS Adding vertex {0}: {1}\n", vertices.size(), glm::to_string(mv.position));

					vertices.push_back(mv);
				}
			}
		}
	}

	// TODO: generate and bind the VAO, then handle the VBO, then set up the vertex attribute pointers, then
	// bind the passed IBO. Look at TriangleMeshSAO::init as an outline, but know that an exact copy
	// of the other mesh's code won't work here. You'll need to modify it. 
	void init(GLuint IBO, GLuint shaderProgram) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), vertices.data(), GL_STATIC_DRAW);

		GLint positionLoc = glGetAttribLocation(shaderProgram, "vPos");
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (const void *)offsetof(MeshVertex, position));  

		//offsetof returns long unsigned int, must cast to (const void*) to work

		glEnableVertexAttribArray(positionLoc);

		GLint colorLoc = glGetAttribLocation(shaderProgram, "vCol");
		glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (const void *)offsetof(MeshVertex, color));
		glEnableVertexAttribArray(colorLoc);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBindVertexArray(0);
	}
};

struct Shader {
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint program = 0;

	bool compileShader(const char* shaderSrc, GLenum shaderType, GLuint& shader) {
		shader = glCreateShader(shaderType);

		std::string shaderText = StringUtil::replaceAll(shaderSrc, "__VERSION__", SoftwareRenderer::shaderVersion);

		const char* shaderTextPtr = shaderText.c_str();
		glShaderSource(shader, 1, &shaderTextPtr, nullptr);
		glCompileShader(shader);

		int  success = 0;
		static char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			log("{0} shader compilation failed:\n{1}\n", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"), infoLog);
			return false;
		}

		return true;
	}

	bool linkProgram() {
		GLint success = 0;

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			log("Shader linking failed:\n{0}\n", infoLog);
			return false;
		}

		return true;
	}

	bool init() {
		const char* vertexShaderSrc = R"VERTEXSHADER(
__VERSION__

uniform mat4 mvp;

in vec3 vPos;
in vec3 vCol;

out vec3 fCol;

void main() {
	fCol = vCol;
	gl_Position = mvp * vec4(vPos.x, vPos.y, vPos.z, 1.0);
}

)VERTEXSHADER";

		const char* fragmentShaderSrc = R"FRAGMENTSHADER(
__VERSION__

uniform vec4 color;

in vec3 fCol;

out vec4 FragColor;

void main() {
	FragColor = color * vec4(fCol, 1.0);
}

)FRAGMENTSHADER";


		if (!compileShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShader)) {
			if (vertexShader != 0) {
				glDeleteShader(vertexShader);
				vertexShader = 0;
			}
			return false;
		}

		if (!compileShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShader)) {
			if (fragmentShader != 0) {
				glDeleteShader(fragmentShader);
				fragmentShader = 0;
			}
			return false;
		}

		if (!linkProgram()) {
			if (program != 0) {
				glDeleteProgram(program);
				program = 0;
			}
			return false;
		}

		return true;
	}
};

TriangleMeshSOA cubeSOA;
TriangleMeshAOS cubeAOS;
IndexBuffer indexBuffer;
Shader shader;

Camera camera;
std::vector<SceneObject> sceneObjects;

void addSceneObject() {
	SceneObject newObject;
	newObject.name = fmt::format("Object {0}", sceneObjects.size() + 1);
	sceneObjects.push_back(newObject);
}

// TODO: edit the default values
void Lab07::init() {

	indexBuffer.init();

	if (!shader.init()) {
		exit(1);
	}

	cubeSOA.init(indexBuffer.IBO, shader.program);
	cubeAOS.init(indexBuffer.IBO, shader.program);

	addSceneObject();

	initialized = true;
}

bool renderSOA = true;

// Renders to the "screen" texture that has been passed in as a parameter
void Lab07::render(s_ptr<Framebuffer> framebuffer) {
	if (!initialized) init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	double deltaTime = Application::get().deltaTime;
	double timeSinceStart = Application::get().timeSinceStart;

	glUseProgram(shader.program);
	if (renderSOA) {
		glBindVertexArray(cubeSOA.VAO);
	}
	else {
		glBindVertexArray(cubeAOS.VAO);
	}

	camera.update(float(framebuffer->width), float(framebuffer->height));

	mat4 vp = camera.projection * camera.view;

	for (auto& sceneObject : sceneObjects) {
		if (glm::length(vec3(sceneObject.transform.rotation)) > 0.f) {
			sceneObject.transform.rotation.w += deltaTime;
		}
		mat4 model = sceneObject.transform.getMatrixGLM();
		mat4 mvp = vp * model;

		GLuint mvpLocation = glGetUniformLocation(shader.program, "mvp");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		GLuint colorLocation = glGetUniformLocation(shader.program, "color");
		glUniform4fv(colorLocation, 1, glm::value_ptr(sceneObject.color));

		glDrawElements(GL_TRIANGLES, indexBuffer.indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Lab07::renderUI() {

	if (ImGui::Button(renderSOA ? "Struct of arrays" : "Array of structs")) {
		renderSOA = !renderSOA;
	}

	camera.renderUI();

	if (ImGui::CollapsingHeader("Scene objects")) {
		IMDENT;

		static int numberToAdd = 10;

		if (ImGui::Button("Add new")) {
			addSceneObject();
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
				newObject.transform.rotation = vec4(glm::sphericalRand(1.0f), glm::linearRand(0.f, glm::two_pi<float>()));
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
