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

#include "MeshBuilder.h"

class Plane {
private:
	MeshBuilder m_mesh;

	glm::ivec2 m_nrOfTiles;
	glm::vec2 m_tileSize;

public:
	Plane();
	~Plane();

	void setNrOfTiles(glm::ivec2 nrOfTiles);
	void setTileSize(glm::vec2 tileSize);
	void generateVertices();
	void setTexture(char* textureFileDiffuse, char* textureFileSpecular);
	void setUvCoords(glm::ivec2 tile, glm::vec2 uvStart, glm::vec2 uvEnd);
	void setTilePositions(glm::ivec2 tile, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4);
	void createBuffers();
	void updateBuffers();

	std::vector<glm::vec3> getTilePositions(glm::ivec2 tile);

	void setModelMatrix(glm::mat4 modelMatrix);

	void draw(GLuint shaderProgram);
};