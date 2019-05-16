#pragma once

#include <GL\glew.h>
#include <SFML\Window.hpp>
#include <SFML\Graphics\Image.hpp>
#include <SFML\OpenGL.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

class GUIElement {
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_texture;

	GLint m_textureLoc, m_modelMatrixLoc;

	struct Vertex {
		float x, y, z; //Position
		float u, v; //TexCoords
	};

	Vertex m_vertices[6]{
		//Position				//TexCoords
		0.0f, 0.0f,  0.0f,		0.0f, 0.0f,
		1.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		1.0f, 0.0f,  0.0f,		1.0f, 0.0f,
		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		1.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.0f, 0.0f,  0.0f,		0.0f, 0.0f,
	};

	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;

	glm::mat4 m_modelMatrix;
	bool m_mmNeedsUpdate = true;

	void createBuffer();
	void updateModelMatrix();

public:
	GUIElement();
	~GUIElement();

	void setTexture(char* textureFile);
	void setPosition(glm::vec2 position);
	void setRotation(float rotation);
	void setSize(glm::vec2 size);
	void setUV(glm::vec2 xy0, glm::vec2 xy1);

	void draw(GLuint shaderProgram);
};