#include "ShaderSet.h"

ShaderSet::ShaderSet(std::string vertexShaderName, std::string fragmentShaderName) {
	shaderProgram = 0;

	createShaders(vertexShaderName, fragmentShaderName);
}
ShaderSet::ShaderSet(std::string vertexShaderName, std::string fragmentShaderName, std::string geometryShaderName) {
	shaderProgram = 0;

	createShaders(vertexShaderName, fragmentShaderName, geometryShaderName);
}

ShaderSet::~ShaderSet() {

}

void ShaderSet::createShaders(std::string vertexShaderName, std::string fragmentShaderName) {
	//Create shaders

	//Vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream shaderFile("res/Shaders/" + vertexShaderName);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	// make a double pointer (only valid here)
	const char* shaderTextPtr = shaderText.c_str();
	// ask GL to load this
	glShaderSource(vertexShader, 1, &shaderTextPtr, nullptr);
	// ask GL to compile it
	glCompileShader(vertexShader);

	//Check for compile-time errors for vertex shader
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderFile.open("res/Shaders/" + fragmentShaderName);
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fragmentShader, 1, &shaderTextPtr, nullptr);
	glCompileShader(fragmentShader);

	//Check for compile-time errors for fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	shaderProgram = glCreateProgram(); //Declared in beginning of program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete shaders - they are not needed anymore after we have bound them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void ShaderSet::createShaders(std::string vertexShaderName, std::string fragmentShaderName, std::string geometryShaderName) {
	//Create shaders

	//Vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream shaderFile("res/Shaders/" + vertexShaderName);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	// make a double pointer (only valid here)
	const char* shaderTextPtr = shaderText.c_str();
	// ask GL to load this
	glShaderSource(vertexShader, 1, &shaderTextPtr, nullptr);
	// ask GL to compile it
	glCompileShader(vertexShader);

	//Check for compile-time errors for vertex shader
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Geometry shader

	GLuint geometryShader;
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	shaderFile.open("res/Shaders/" + geometryShaderName);
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(geometryShader, 1, &shaderTextPtr, nullptr);
	glCompileShader(geometryShader);

	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderFile.open("res/Shaders/" + fragmentShaderName);
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fragmentShader, 1, &shaderTextPtr, nullptr);
	glCompileShader(fragmentShader);

	//Check for compile-time errors for fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	shaderProgram = glCreateProgram(); //Declared in beginning of program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete shaders - they are not needed anymore after we have bound them
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

GLuint ShaderSet::getShaderProgram() {
	return shaderProgram;
}

void ShaderSet::use() {
	glUseProgram(shaderProgram);
}