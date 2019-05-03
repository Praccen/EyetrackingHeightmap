#pragma once

#include <math.h>

#include "../Engine/Application.h"

class Heightmap {
private:
	Object* m_ground;
	glm::ivec2 m_heightmapSize;

public:
	Heightmap();
	~Heightmap();

	glm::vec2 calculateMousePos(glm::vec3 mouseRay, glm::vec3 cameraPos) const;
	void raiseGround(glm::vec2 heightmapCoords, float amount, float radius);
	void lowerGround(glm::vec2 heightmapCoords, float amount, float radius);

	void paintGround(glm::vec2 heightmapCoords, int material, float radius);

};