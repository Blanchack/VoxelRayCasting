#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera {
public:

	Camera() = default;
	~Camera() = default;

	void cameraDataToShader(Shader& shader);
	void cameraInput(GLFWwindow* window);


private:

	glm::vec3 cameraLoc;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;

	float theta;
	float phi;
};