#pragma once
#include <string>

class Shader{
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	unsigned int const getId();

};