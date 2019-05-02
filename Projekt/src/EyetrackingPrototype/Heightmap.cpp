#include "Heightmap.h"
#include "../Engine/Physics/Intersections.h"

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