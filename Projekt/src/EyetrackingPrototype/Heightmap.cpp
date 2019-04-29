#include "Heightmap.h"

Heightmap::Heightmap() {
	m_ground = Application::getNewObject();
	m_ground->setPlaneActive(true);
	m_ground->getPlane()->setTexture("res/Textures/terrain_diffuse.png", "res/Textures/terrain_specular.png");
	m_ground->getPlane()->setNrOfTiles(glm::ivec2(100, 100));
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