#include "Camera.h"

#include<glad/glad.h>

Camera::Camera() {
	cameraLoc = glm::vec3(224.0f, 0.0f, 0.0f);

	cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	cameraUp = glm::cross(cameraDirection, cameraRight);
}

void Camera::cameraDataToShader(Shader& shader){
	int cameraLocLocation = glad_glGetUniformLocation(shader.getId(), "cameraLoc");
	glProgramUniform3fv(shader.getId(), cameraLocLocation, 1, glm::value_ptr(cameraLoc));

	int cameraDirLocation = glad_glGetUniformLocation(shader.getId(), "cameraDirection");
	glProgramUniform3fv(shader.getId(), cameraDirLocation, 1, glm::value_ptr(cameraDirection));

	int cameraUpLocation = glad_glGetUniformLocation(shader.getId(), "cameraUp");
	glProgramUniform3fv(shader.getId(), cameraUpLocation, 1, glm::value_ptr(cameraUp));

	int cameraRightLocation = glad_glGetUniformLocation(shader.getId(), "cameraRight");
	glProgramUniform3fv(shader.getId(), cameraRightLocation, 1, glm::value_ptr(cameraRight));
}

void Camera::cameraInput(GLFWwindow* window){
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

}

