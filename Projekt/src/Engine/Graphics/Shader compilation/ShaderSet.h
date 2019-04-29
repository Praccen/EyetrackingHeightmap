#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#include <SFML\OpenGL.hpp>

#include <iostream>
#include <fstream>

class ShaderSet {
private:
	GLuint shaderProgram; //Shader program variable
	void createShaders(std::string vertexShaderName, std::string fragmentShaderName, std::string geometryShaderName);
	void createShaders(std::string vertexShaderName, std::string fragmentShaderName);
public:
	ShaderSet(std::string vertexShaderName, std::string fragmentShaderName);
	ShaderSet(std::string vertexShaderName, std::string fragmentShaderName, std::string geometryShaderName);
	~ShaderSet();

	GLuint getShaderProgram();
	void use();
};

#endif