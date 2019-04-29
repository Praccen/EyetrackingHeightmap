#include "Heightmap.h"

Heightmap::Heightmap() {
	m_heightmapSize = glm::ivec2(100, 100);

	m_ground = Application::getNewObject();
	m_ground->setPlaneActive(true);
	m_ground->getPlane()->setTexture("res/Textures/terrain_diffuse.png", "res/Textures/terrain_specular.png");
	m_ground->getPlane()->setNrOfTiles(m_heightmapSize);
	m_ground->getPlane()->setTileSize(glm::vec2(1.0f, 1.0f));
	m_ground->getPlane()->generateVertices();
	m_ground->getPlane()->createBuffers();

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			m_ground->getPlane()->setUvCoords(glm::ivec2(i, j), glm::vec2(0.2, 0.0), glm::vec2(0.4, 1.0));
		}
	}

	m_ground->getPlane()->updateBuffers();
}

Heightmap::~Heightmap() {

}

void Heightmap::raiseGround(glm::vec2 heightmapCoords) {
	
}

void Heightmap::lowerGround(glm::vec2 heightmapCoords) {

}

void Heightmap::paintGround(glm::vec2 heightmapCoords, int material, float radius) {
	for (int i = std::max((int)floor(heightmapCoords.x - radius), 0); i < std::min((int)round(heightmapCoords.x + radius) + 1, m_heightmapSize.x); i++) {
		for (int j = std::max((int)floor(heightmapCoords.y - radius), 0); j < std::min((int)round(heightmapCoords.y + radius) + 1, m_heightmapSize.y); j++) {
			if (glm::length(glm::vec2((float)i + 0.5, (float)j + 0.5) - heightmapCoords) <= radius) {
				m_ground->getPlane()->setUvCoords(glm::ivec2(i, j), glm::vec2(material * 0.2, 0.0), glm::vec2((material + 1) * 0.2, 1.0));
			}
		}
	}

	m_ground->getPlane()->updateBuffers();
}