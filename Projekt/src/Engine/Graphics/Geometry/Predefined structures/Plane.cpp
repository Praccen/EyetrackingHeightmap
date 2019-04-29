#include "Plane.h"

Plane::Plane() {
	m_nrOfTiles = glm::ivec2(1);
	m_tileSize = glm::vec2(1.0f);
}

Plane::~Plane() {

}

void Plane::setNrOfTiles(glm::ivec2 nrOfTiles) {
	m_nrOfTiles = nrOfTiles;
}

void Plane::setTileSize(glm::vec2 tileSize) {
	m_tileSize = tileSize;
}

void Plane::generateVertices() {
	for (int j = 0; j < m_nrOfTiles.y; j++) {
		for (int i = 0; i < m_nrOfTiles.x; i++) {
			m_mesh.addTriangle(
				glm::vec3(i * m_tileSize.x, 0.0f, j * m_tileSize.y), //Pos0
				glm::vec3(i * m_tileSize.x, 0.0f, j * m_tileSize.y + m_tileSize.y), //Pos1
				glm::vec3(i * m_tileSize.x + m_tileSize.x, 0.0f, j * m_tileSize.y), //Pos2
				glm::vec2(0.0f, 0.0f), //Uv0
				glm::vec2(0.0f, 1.0f), //Uv1
				glm::vec2(1.0f, 0.0f) //Uv2
			);

			m_mesh.addTriangle(
				glm::vec3(i * m_tileSize.x + m_tileSize.x, 0.0f, j * m_tileSize.y), //Pos0
				glm::vec3(i * m_tileSize.x, 0.0f, j * m_tileSize.y + m_tileSize.y), //Pos1
				glm::vec3(i * m_tileSize.x + m_tileSize.x, 0.0f, j * m_tileSize.y + m_tileSize.y), //Pos2
				glm::vec2(1.0f, 0.0f), //Uv0
				glm::vec2(0.0f, 1.0f), //Uv1
				glm::vec2(1.0f, 1.0f) //Uv2
			);

			/*
			0---------2/3
			|          /|
			|        /  |
			|      /    |
			|    /      |
			|  /        |
			|/          |
			1/4---------5
			*/
		}
	}
}

void Plane::setTexture(char* textureFileDiffuse, char* textureFileSpecular) {
	m_mesh.setTexture(textureFileDiffuse, textureFileSpecular);
}

void Plane::setUvCoords(glm::ivec2 tile, glm::vec2 uvStart, glm::vec2 uvEnd) {
	if (tile.x < m_nrOfTiles.x && tile.y < m_nrOfTiles.y) {
		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6, uvStart);

		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 1, glm::vec2(uvStart.x, uvEnd.y));

		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 2, glm::vec2(uvEnd.x, uvStart.y));

		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 3, glm::vec2(uvEnd.x, uvStart.y));

		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 4, glm::vec2(uvStart.x, uvEnd.y));

		m_mesh.setVertexUvCoords(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 5, uvEnd);

		/*
		0---------2/3
		|          /|
		|        /  |
		|      /    |
		|    /      |
		|  /        |
		|/          |
		1/4---------5
		*/
	}
}

void Plane::setTilePositions(glm::ivec2 tile, glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3) {
	glm::vec3 normal1, normal2;

	glm::vec3 vec1, vec2;

	/*vec1 = pos1 - pos0;
	vec2 = pos2 - pos0;
	normal1 = glm::normalize(glm::cross(vec1, vec2));

	vec1 = pos1 - pos2;
	vec2 = pos3 - pos2;
	normal2 = glm::normalize(glm::cross(vec1, vec2));*/

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6, pos0);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6, normal1);

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 1, pos1);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 1, normal1);

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 2, pos2);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 2, normal1);

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 3, pos2);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 3, normal2);

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 4, pos1);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 4, normal2);

	m_mesh.setVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 5, pos3);
	//m_mesh.setVertexNormal(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 5, normal2);


		/*
		0-----------2
		|          /|
		|        /  |
		|      /    |
		|    /      |
		|  /        |
		|/          |
		1-----------3
		*/
}

void Plane::createBuffers() {
	m_mesh.createBuffers();
}

void Plane::updateBuffers() {
	m_mesh.updateBuffers();
}

std::vector<glm::vec3> Plane::getTilePositions(glm::ivec2 tile) {
	std::vector<glm::vec3> positions;
	if (tile.x < m_nrOfTiles.x && tile.y < m_nrOfTiles.y) {
		positions.push_back(m_mesh.getVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6));
		positions.push_back(m_mesh.getVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 1));
		positions.push_back(m_mesh.getVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 2));
		positions.push_back(m_mesh.getVertexPosition(m_nrOfTiles.x * 6 * tile.y + tile.x * 6 + 5));
	}
	return positions;
}

void Plane::setModelMatrix(glm::mat4 modelMatrix) {
	m_mesh.setModelMatrix(modelMatrix);
}

void Plane::draw(GLuint shaderProgram) {
	m_mesh.draw(shaderProgram);
}