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

	void raiseGround(glm::vec2 heightmapCoords);
	void lowerGround(glm::vec2 heightmapCoords);

	void paintGround(glm::vec2 heightmapCoords, int material, float radius);

};