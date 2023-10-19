#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragPath){
	std::string vertexCode, fragCode;
	std::ifstream vertexStream, fragStream;

	vertexStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try{
		vertexStream.open(vertexPath);
		fragStream.open(fragPath);

		std::stringstream vertexSS, fragSS;
		vertexSS << vertexStream.rdbuf();
		fragSS << fragStream.rdbuf();

		vertexCode = vertexSS.str();
		fragCode = fragSS.str();

		std::cout << fragCode << std::endl << vertexCode;

		vertexStream.close();
		fragStream.close();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	unsigned int fragShader;
	unsigned int vertexShader;

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertexSource = vertexCode.c_str();
	const char* fragSource = fragCode.c_str();
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	id = glCreateProgram();
	glAttachShader(id, fragShader);
	glAttachShader(id, vertexShader);
	glLinkProgram(id);

	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::use(){
	glUseProgram(id);
}


unsigned const Shader::getId(){
	return id;
}
