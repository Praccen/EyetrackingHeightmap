#pragma once

#include <GL\glew.h>
#include <SFML\Window.hpp>
#include <SFML\Graphics\Image.hpp>
#include <SFML\OpenGL.hpp>

#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\type_ptr.hpp>

class MeshBuilder {
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_textureDiffuse;
	GLuint m_textureSpecular;

	//Geometry uniform locations
	GLint m_textureDiffuseLoc, m_textureSpecularLoc, m_geometryModelMatrixLoc, m_normalMatrixLoc;

	//Shadow uniform locations
	GLint m_shadowModelMatrixLoc;

	struct Vertex {
		float x, y, z; //Position
		float nx, ny, nz; //Normal
		float u, v; //TexCoords
	};

	int m_nrOfVertices;
	glm::mat4 m_modelMatrix;
	glm::mat3 m_normalMatrix;

	std::vector<Vertex> m_vertices;
	std::vector<int> m_freeArraySpaces;

	bool m_readyToDraw;

public:
	MeshBuilder();
	~MeshBuilder();

	int addTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2);
	void setTexture(char* textureFileDiffuse, char* textureFileSpecular);
	void setVertexPosition(int vertexIndex, glm::vec3 pos);
	void setVertexNormal(int vertexIndex, glm::vec3 normal);
	void setVertexUvCoords(int vertexIndex, glm::vec2 uv);
	void removePrimative(int startingIndex);
	void createBuffers();
	void updateBuffers();

	glm::vec3 getVertexPosition(int vertexIndex) const;
	glm::vec3 getVertexNormal(int vertexIndex) const;
	int getNumberOfVertices() const;

	void setModelMatrix(glm::mat4 modelMatrix);

	void draw(GLuint shaderProgram);
};