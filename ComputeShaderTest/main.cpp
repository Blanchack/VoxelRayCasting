#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glm::vec3& cameraRight, glm::vec3& cameraUp, glm::vec3& cameraLoc, glm::vec3& cameraDirection, float& phi, float& theta);

float const WIDTH = 800;
float const HEIGHT = 600;

struct Voxel
{
	glm::vec4 pos;
	glm::vec4 col;
};

void loadModel(std::string fileDir, std::vector<Voxel>& data) {
	std::ifstream inFile;
	inFile.open(fileDir);

	std::ifstream archivo(fileDir);

	if (archivo.is_open()) {
		std::string linea;

		while (std::getline(archivo, linea)) {
			std::vector<float> valores;
			std::stringstream ss(linea);
			std::string valor;

			while (std::getline(ss, valor, ',')) {
				valores.push_back(std::stoi(valor));
			}

			if (valores.size() == 6) {
				Voxel voxel = { glm::vec4(valores[0], valores[1], valores[2], 1.0f), glm::vec4(valores[3], valores[4], valores[5], 1.0f) };
				data.push_back(voxel);
				valores.clear();
			}
			else {
				std::cerr << "Error: Se esperaban 6 valores por línea en el archivo." << std::endl;
				break;
			}
		}

		archivo.close();
	}
	else {
		std::cerr << "No se pudo abrir el archivo." << std::endl;
	}



}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Compute Shader Test", NULL, NULL);

	if (window == NULL) {
		std::cout << "Error creaando la ventana GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Cago GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		//upper-left triangle
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, 1.0f, 0.0f,
		 //lower-right triangle
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};


	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int vertexArray;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	unsigned int vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	Shader shader("Vertex.glsl", "Fragment.glsl");
	shader.use();

	glm::vec2 resolution = glm::vec2(WIDTH, HEIGHT);
	int VPDlocation = glGetUniformLocation(shader.getId(), "windowDimensions");
	glProgramUniform2fv(shader.getId(), VPDlocation, 1, glm::value_ptr(resolution));

	glm::vec3 cameraLoc = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	int cameraLocLocation = glad_glGetUniformLocation(shader.getId(), "cameraLoc");
	glProgramUniform3fv(shader.getId(), cameraLocLocation, 1, glm::value_ptr(cameraLoc));

	int cameraDirLocation = glad_glGetUniformLocation(shader.getId(), "cameraDirection");
	glProgramUniform3fv(shader.getId(), cameraDirLocation, 1, glm::value_ptr(cameraDirection));

	int cameraUpLocation = glad_glGetUniformLocation(shader.getId(), "cameraUp");
	glProgramUniform3fv(shader.getId(), cameraUpLocation, 1, glm::value_ptr(cameraUp));

	int cameraRightLocation = glad_glGetUniformLocation(shader.getId(), "cameraRight");
	glProgramUniform3fv(shader.getId(), cameraRightLocation, 1, glm::value_ptr(cameraRight));

	std::vector<Voxel> voxelData;

	/*Voxel a = {glm::vec4(4.0f,-1.0f,8.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)};
	Voxel b = { glm::vec4(1.0f,-3.0f,4.0f,1.0f), glm::vec4(0.3f, 0.8f, 0.6f, 1.0f) };
	Voxel c = { glm::vec4(0.0f,1.0f,3.0f,1.0f), glm::vec4(0.0f, 0.2f, 0.5f, 1.0f) };
	Voxel d = { glm::vec4(-2.0f,-1.0f,3.0f,1.0f), glm::vec4(0.0f, 1.0f, 0.5f, 1.0f) };
	Voxel e = { glm::vec4(-3.0f, 0.0f, 3.0f, 1.0f), glm::vec4(1.0f, 0.39f , 0.52f, 1.0f) };

	voxelData.push_back(a);
	voxelData.push_back(b);
	voxelData.push_back(c);
	voxelData.push_back(d);
	voxelData.push_back(e);*/

	loadModel("teapot_128x63x80.txt", voxelData);

	unsigned int SSBO;


	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, voxelData.size() * sizeof(Voxel), voxelData.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO);

	float phi = 0.0f;
	float theta = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		cameraDirection = glm::vec3(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		processInput(window, cameraRight, cameraUp, cameraLoc, cameraDirection, phi, theta);

		// render
		// ------
		int cameraLocLocation = glad_glGetUniformLocation(shader.getId(), "cameraLoc");
		glProgramUniform3fv(shader.getId(), cameraLocLocation, 1, glm::value_ptr(cameraLoc));

		int cameraDirLocation = glad_glGetUniformLocation(shader.getId(), "cameraDirection");
		glProgramUniform3fv(shader.getId(), cameraDirLocation, 1, glm::value_ptr(cameraDirection));

		int cameraUpLocation = glad_glGetUniformLocation(shader.getId(), "cameraUp");
		glProgramUniform3fv(shader.getId(), cameraUpLocation, 1, glm::value_ptr(cameraUp));

		int cameraRightLocation = glad_glGetUniformLocation(shader.getId(), "cameraRight");
		glProgramUniform3fv(shader.getId(), cameraRightLocation, 1, glm::value_ptr(cameraRight));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first 
		glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, glm::vec3& cameraRight, glm::vec3& cameraUp, glm::vec3& cameraLoc, glm::vec3& cameraDirection, float& phi, float&  theta) {

	const float movSpeedMult = 8.0f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraLoc += cameraDirection * movSpeedMult;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraLoc -= cameraDirection * movSpeedMult;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraLoc -= cameraRight * movSpeedMult;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraLoc += cameraRight * movSpeedMult;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraLoc += glm::vec3(0.0f, 0.1f, 0.0f) * 15.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraLoc -= glm::vec3(0.0f, 0.1f, 0.0f) * 15.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraLoc += cameraRight * movSpeedMult;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		theta += 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		theta -= 0.02f;
	}
	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		phi += 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		phi -= 0.02f;
	}*/
}