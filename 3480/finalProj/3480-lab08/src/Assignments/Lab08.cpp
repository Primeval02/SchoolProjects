#include "Lab08.h"

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

struct OBJMeshVertex {
	vec3 position;
	vec3 normal;
	vec2 texCoord;		// unused for this lab
};

struct OBJMesh {
	std::vector<OBJMeshVertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint IBO = 0;

	bool isValid() {
		return VAO != 0 && VBO != 0 && IBO != 0 && !vertices.empty() && !indices.empty();
	}

	static OBJMesh import(const char* objFile, GLuint shaderProgram) {
		OBJMesh mesh;

		std::ifstream fin(objFile);

		if (!fin) {
			log("Error creating input stream on {0}\n", objFile);
			return mesh;
		}

		std::string line;
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		GLuint faceIndex = 0;

		// TODO: parse the contents of the OBJ file line by line
		while (std::getline(fin, line)) {
			//log("{0}\n", line);

			auto lineParts = StringUtil::split(line, ' ');

			if (lineParts.empty()) continue;

			if (lineParts[0] == "v" || lineParts[0] == "vn") {
				vec3 v;
				v[0] = std::atof(lineParts[1].c_str());
				v[1] = std::atof(lineParts[2].c_str());
				v[2] = std::atof(lineParts[3].c_str());

				if (lineParts[0] == "v") {
					vertices.push_back(v);
				}

				if (lineParts[0] == "vn") {
					normals.push_back(v);
				}
			}

			else if (line[0] == 'f') {
				if (StringUtil::contains(line, "/")) {
					for (int i = 1; i < lineParts.size(); i++) {
						int positionIndex = 0, normalIndex = 0;

						auto faceComponents = StringUtil::split(lineParts[i], '/');

						if (faceComponents.size() == 2) {
							positionIndex = std::stoi(faceComponents[0]) - 1;
							normalIndex = std::stoi(faceComponents[1]) - 1;
						}

						else if (faceComponents.size() == 3) {
							positionIndex = std::stoi(faceComponents[0]) - 1;
							normalIndex = std::stoi(faceComponents[2]) - 1;
						}

						OBJMeshVertex omv;
						omv.position = vertices[positionIndex];
						omv.normal = normals[normalIndex];

						mesh.vertices.push_back(omv);
						mesh.indices.push_back(faceIndex++);

					}
				}
				else {
					for(int i = 1; i < lineParts.size(); i++) {
						int positionIndex = std::stoi(lineParts[i]) - 1;

						OBJMeshVertex omv;
						omv.position = vertices[positionIndex];
						omv.normal = vec3(0.f);

						mesh.vertices.push_back(omv);
						mesh.indices.push_back(faceIndex++);
					}
				}
			}
		}

		// TODO: initialize the mesh data on the GPU before returning the mesh
		glBufferData(GL_ARRAY_BUFFER, sizeof(OBJMeshVertex) * mesh.vertices.size(), (const void*)mesh.vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &mesh.VAO);
		glBindVertexArray(mesh.VAO);


		glGenBuffers(1, &mesh.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(OBJMeshVertex) * mesh.vertices.size(), (const void*)mesh.vertices.data(), GL_STATIC_DRAW);

		GLint positionLocation = glGetAttribLocation(shaderProgram, "vPosition");
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(OBJMeshVertex), (const void*)offsetof(OBJMeshVertex, position));
		glEnableVertexAttribArray(positionLocation);

		GLint normalLocation = glGetAttribLocation(shaderProgram, "vNormal");
		if (normalLocation >= 0) {
			glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(OBJMeshVertex), (const void*)offsetof(OBJMeshVertex, normal));
			glEnableVertexAttribArray(normalLocation);
		}
		glGenBuffers(1, &mesh.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), (const void*)mesh.indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);

		return mesh;
	}

	static OBJMesh getSphere(GLuint shaderProgram) {
		OBJMesh sphere;
		glGenVertexArrays(1, &sphere.VAO);
		glBindVertexArray(sphere.VAO);

		glGenBuffers(1, &sphere.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, sphere.VBO);

		for (auto& vertex : Sphere::instance.positions) {
			OBJMeshVertex omv;
			omv.position = vertex;
			omv.normal = glm::normalize(omv.position);

			sphere.vertices.push_back(omv);
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(OBJMeshVertex) * sphere.vertices.size(), (const void*)sphere.vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &sphere.IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.IBO);
		for (auto& tri : Sphere::instance.indices) {
			for (int i = 0; i < 3; i++) {
				sphere.indices.push_back(tri[i]);
			}
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * sphere.indices.size(), (const void*)sphere.indices.data(), GL_STATIC_DRAW);

		GLint positionLocation = glGetAttribLocation(shaderProgram, "vPosition");
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(OBJMeshVertex), (const void*)offsetof(OBJMeshVertex, position));
		glEnableVertexAttribArray(positionLocation);

		GLint normalLocation = glGetAttribLocation(shaderProgram, "vNormal");
		if (normalLocation >= 0) {
			glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(OBJMeshVertex), (const void*)offsetof(OBJMeshVertex, normal));
			glEnableVertexAttribArray(normalLocation);
		}
		glBindVertexArray(0);

		return sphere;
	}
};

std::vector<OBJMesh> meshes;

int activeMeshIndex = 0;

// TODO: update the vertex and fragment shaders to use Blinn-Phong shading.
void Lab08::init() {

	const char* Lab08VertexShaderSrc = R"VERTEXSHADER(
__VERSION__

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalMatrix;

in vec3 vPosition;
in vec3 vNormal;

out vec3 fPos;
out vec3 fNormal;

void main() {
	fPos = vec3(model * vec4(vPosition, 1.0));
	fNormal = normalize(normalMatrix * vNormal);
	gl_Position = mvp * vec4(vPosition, 1.0);
}

)VERTEXSHADER";

	const char* Lab08FragmentShaderSrc = R"FRAGMENTSHADER(
__VERSION__

const float PI = 3.1415926535897932384626433832795;

uniform vec4 color;
uniform vec3 viewPosition;
uniform vec3 lightDirection;
uniform float shininess; 
uniform float roughness; // alpha in equations                 Material properties for PBR
uniform float albedo; // c in equations
uniform float metalness; // straightforward
uniform float Ia, Id;
uniform vec3 Ka, Kd, Ks;

in vec3 fPos;
in vec3 fNormal;

out vec4 FragColor;

// Vital BRDF components: 

// Using GGX normal distribution function
float Distribution(vec3 n, vec3 h, float a) {
	float a2 = a * a;
	float NdotH = max(dot(n, h), 0.0);
	float NdotH2 = NdotH * NDotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

// Sub part for geometry function  using Schlick-GGX
float GeometrySchlick(float NdotV, float k) {
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom /denom;
}

// Geometry function main using Smith
float GeometrySmith(vec3 n, vec3 v, vec3 l, float k) {
	float NdotV = max(dot(n, v), 0.0);
	float NdotL = max(dot(n, l), 0.0);
	float ggx1 = GeometrySchlick(NdotV, k);
	float ggx2 = GeometrySchlick(NdotL, k);

	return ggx1 * ggx2;
}

// Fresnel equation using Schlick
vec3 Fresnel(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {

	//ambient
	vec3 ambient = Ia * Ka;

	//diffuse TAKING NORM AND LIGHTDIR FOR PBR
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(-lightDirection);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * Kd * Id;  //this calc is identical to Li(p, wi) or incoming/input radiance

	//specular   TAKING VIEWDIR AND HAL DIR FOR PBR
	vec3 viewDir = normalize(viewPosition - fPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	//vec3 specular = spec * Ks;

	//vec3 finalColor = (ambient + diffuse + specular) * color.rgb;
	//FragColor = vec4(finalColor, 1.0);


	// Begin PBR implementation
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, color.rgb, metalness);

	float cosTheta = dot(lightDir, norm);

	// Cook-Torrance BRDF has diffuse and specular: fr = kd * flam + ks * fcook
	// lambert diffuse should be more than enough for most situations

	// diffuse
	float flam = albedo / PI;
	flam = flam * Kd;

	// specular
	float k = ((roughness + 1) * (roughness + 1)) / 8; // k is a remapping of alpha using direct lighting
	float fcook = (Distribution(norm, halfwayDir, roughness) * GeometrySmith(norm, viewDir, k) * Fresnel(cosTheta, F0) / (4.0 * (dot(lightDir * norm)) * dot(-lightDir * norm)));
	fcook = fcook * Ks;

	float fr = flam + fcook;

	// Loop should calculate BRDF on different points of hemisphere and accumulates. essentially outgoing radiance
	int steps = 100;
	float sum = 0.0f;
	float dW = 1.0f / steps;
	for (int i = 0; i < steps; i++) {
		sum += fr * diffuse * dot(norm, lightDir) * dW;   // Li, middle term, is input radiance
	}

}

)FRAGMENTSHADER";

	if (!shader.init(Lab08VertexShaderSrc, Lab08FragmentShaderSrc)) {
		exit(1);
	}

// Default sphere mesh
OBJMesh sphereMesh = OBJMesh::getSphere(shader.program);
meshes.push_back(sphereMesh);

addSceneObject();

initialized = true;
}

// Renders to the "screen" texture that has been passed in as a parameter
void Lab08::render(s_ptr<Framebuffer> framebuffer) {
	if (!initialized) init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	double deltaTime = Application::get().deltaTime;
	double timeSinceStart = Application::get().timeSinceStart;

	glUseProgram(shader.program);

	OBJMesh* activeMesh = &meshes[activeMeshIndex];
	glBindVertexArray(activeMesh->VAO);

	camera.update(float(framebuffer->width), float(framebuffer->height));

	if (light.autoOrbit) {
		vec3 forward = vec3(0, 0, -1);

		if (glm::abs(glm::dot(forward, light.orbitAxis)) > 0.99f) {
			forward = vec3(1, 0, 0);
		}

		mat4 orbit = glm::rotate((float)timeSinceStart, light.orbitAxis);

		light.lightDirection = orbit * vec4(forward, 0);
	}

	mat4 vp = camera.projection * camera.view;

	for (auto& sceneObject : sceneObjects) {
		if (glm::length(vec3(sceneObject.transform.rotation)) > 0.f) {
			sceneObject.transform.rotation.w += deltaTime;
		}
		mat4 model = sceneObject.transform.getMatrixGLM();
		mat4 mvp = vp * model;
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

		GLuint mvpLocation = glGetUniformLocation(shader.program, "mvp");
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		// TODO: update uniform variables in shader required for Blinn-Phong shading.
		// Ambient light intensity and color are already being passed in.
		GLint iaLocation = glGetUniformLocation(shader.program, "Ia");
		glUniform1f(iaLocation, light.Ia);

		GLint idLocation = glGetUniformLocation(shader.program, "Id");
		glUniform1f(idLocation, light.Id);

		GLint kaLocation = glGetUniformLocation(shader.program, "Ka");
		glUniform3fv(kaLocation, 1, glm::value_ptr(light.Ka));

		GLint kdLocation = glGetUniformLocation(shader.program, "Kd");
		glUniform3fv(kdLocation, 1, glm::value_ptr(light.Kd));

		GLint ksLocation = glGetUniformLocation(shader.program, "Ks");
		glUniform3fv(ksLocation, 1, glm::value_ptr(light.Ks));

		GLint colorLocation = glGetUniformLocation(shader.program, "color");
		glUniform4fv(colorLocation, 1, glm::value_ptr(sceneObject.color));

		GLuint lightDirLocation = glGetUniformLocation(shader.program, "lightDirection");
		glUniform3fv(lightDirLocation, 1, glm::value_ptr(light.lightDirection));

		GLuint viewPosLocation = glGetUniformLocation(shader.program, "viewPosition");
		glUniform3fv(viewPosLocation, 1, glm::value_ptr(camera.cameraPosition));

		GLuint shininessLocation = glGetUniformLocation(shader.program, "shininess");
		glUniform1f(shininessLocation, light.shininess);

		GLuint modelLocation = glGetUniformLocation(shader.program, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLuint normalMatrixLocation = glGetUniformLocation(shader.program, "normalMatrix");
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glDrawElements(GL_TRIANGLES, activeMesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Lab08::renderUI() {

	int numMeshes = meshes.size();
	ImGui::Text("Number of meshes: %d", numMeshes);

	int meshMax = meshes.size() - 1;

	if (meshMax > 0) {
		ImGui::SliderInt("Active mesh index", &activeMeshIndex, 0, meshMax);
	}

	if (ImGui::Button("Load model")) {
		ImGuiFileDialog::Instance()->OpenDialog("ChooseOBJKey", "Choose OBJ", ".obj", ".");
	}

	light.renderUI();
	camera.renderUI();

	if (ImGuiFileDialog::Instance()->Display("ChooseOBJKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string objFile = ImGuiFileDialog::Instance()->GetFilePathName();
			OBJMesh loadedMesh = OBJMesh::import(objFile.c_str(), shader.program);

			if (loadedMesh.isValid()) {
				meshes.push_back(loadedMesh);
			}

			ImGuiFileDialog::Instance()->Close();
		}
	}

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
		static vec2 scaleRange = vec2(0.1, 8.f);
		if (ImGui::Button(addRandomLabel.c_str())) {
			for (int i = 0; i < glm::max(numberToAdd, 0); i++) {
				SceneObject newObject;
				newObject.name = fmt::format("Object {0}", sceneObjects.size() + 1);

				newObject.transform.translation = glm::linearRand(vec3(-200), vec3(200));
				newObject.transform.rotation = vec4(glm::sphericalRand(1.0f), glm::linearRand(0.f, glm::two_pi<float>()));
				newObject.transform.scale = vec3(glm::linearRand(scaleRange.x, scaleRange.y));
				newObject.color = glm::linearRand(vec3(0.1f), vec3(1.f));

				sceneObjects.push_back(newObject);
			}
		}


		ImGui::InputFloat2("Scale min/max", glm::value_ptr(scaleRange));

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
