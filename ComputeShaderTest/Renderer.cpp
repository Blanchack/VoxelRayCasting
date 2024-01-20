#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

Renderer::Renderer(){
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Compute Shader Test", NULL, NULL);

	if (window == NULL) {
		std::cout << "Error creaando la ventana GLFW" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Cago GLAD" << std::endl;
		return;
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
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

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

	shader = Shader("Vertex.glsl", "Fragment.glsl");
	shader.use();

	glm::vec2 resolution = glm::vec2(WIDTH, HEIGHT);
	int VPDlocation = glGetUniformLocation(shader.getId(), "windowDimensions");
	glProgramUniform2fv(shader.getId(), VPDlocation, 1, glm::value_ptr(resolution));

	camera.cameraDataToShader(shader);



}

void Renderer::startRender()
{
}