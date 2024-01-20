#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"


class Renderer
{
public:
	Renderer();

	void startRender();

	~Renderer();

private:

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;
	Camera camera;
	Shader shader;
	Model model;

	unsigned int const WIDTH = 800;
	unsigned int const HEIGHT = 600;
};
