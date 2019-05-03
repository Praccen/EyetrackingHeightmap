#include "Heightmap.h"
#include "../Engine/Physics/Intersections.h"

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

void Heightmap::raiseGround(glm::vec2 heightmapCoords, float amount, float radius) {
	for (int i = std::max((int)floor(heightmapCoords.x - radius), 0); i < std::min((int)ceil(heightmapCoords.x + radius) + 1, m_heightmapSize.x); i++) {
		for (int j = std::max((int)floor(heightmapCoords.y - radius), 0); j < std::min((int)ceil(heightmapCoords.y + radius) + 1, m_heightmapSize.y); j++) {
			std::vector<glm::vec3> tempPositions = m_ground->getPlane()->getTilePositions(glm::ivec2(i, j));
			for (int k = 0; k < tempPositions.size(); k++) {
				float tempLength = glm::length(glm::vec2(tempPositions[k].x, tempPositions[k].z) - heightmapCoords);
				if (tempLength <= radius) {
					tempPositions[k].y += amount * std::cos((tempLength / radius) * 1.57f);
				}
			}
			m_ground->getPlane()->setTilePositions(glm::ivec2(i, j), tempPositions[0], tempPositions[1], tempPositions[2], tempPositions[3]);
		}
	}

	m_ground->getPlane()->updateBuffers();
}

void Heightmap::lowerGround(glm::vec2 heightmapCoords, float amount, float radius) {
	for (int i = std::max((int)floor(heightmapCoords.x - radius), 0); i < std::min((int)ceil(heightmapCoords.x + radius) + 1, m_heightmapSize.x); i++) {
		for (int j = std::max((int)floor(heightmapCoords.y - radius), 0); j < std::min((int)ceil(heightmapCoords.y + radius) + 1, m_heightmapSize.y); j++) {
			std::vector<glm::vec3> tempPositions = m_ground->getPlane()->getTilePositions(glm::ivec2(i, j));
			for (int k = 0; k < tempPositions.size(); k++) {
				float tempLength = glm::length(glm::vec2(tempPositions[k].x, tempPositions[k].z) - heightmapCoords);
				if (tempLength <= radius) {
					tempPositions[k].y -= amount * std::cos((tempLength / radius) * 1.57f);
				}
			}
			m_ground->getPlane()->setTilePositions(glm::ivec2(i, j), tempPositions[0], tempPositions[1], tempPositions[2], tempPositions[3]);
		}
	}

	m_ground->getPlane()->updateBuffers();
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
}

glm::vec2 Heightmap::calculateMousePos(glm::vec3 mouseRay, glm::vec3 cameraPos) const{

	float length = 10000.f;
	glm::vec2 pos = glm::vec2(-1.f, -1.f);
	

	//Calculate the intersection and select the closest
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			std::vector<glm::vec3> tile = m_ground->getPlane()->getTilePositions(glm::ivec2(i, j));
			glm::vec3 tileArray[3] = { tile[0], tile[1], tile[2] };
			float tempLength = Intersections::rayTriTest(cameraPos, mouseRay, tileArray);
			glm::vec3 tileArray2[3] = { tile[1], tile[3], tile[2] };
			float tempLength2 = Intersections::rayTriTest(cameraPos, mouseRay, tileArray2);
			if(tempLength != -1.f){
				if (tempLength < length) {
					length = tempLength;
				}
			}
			if (tempLength2 != -1.f) {
				if (tempLength2 < length) {
					length = tempLength2;
				}
			}
		}
	}

	//If there was an intersection, calculate the posistion on the heightmap
	if (length < 10000.f) {
		glm::vec3 tempPos = cameraPos + mouseRay * length;
		pos.x = tempPos.x;
		pos.y = tempPos.z;
	}

	
	return pos;


}