#pragma once

#include "../Engine/Application.h"

class Heightmap {
private:
	Object* m_ground;


public:
	Heightmap();
	~Heightmap();

	glm::vec2 calculateMousePos(glm::vec3 mouseRay, glm::vec3 cameraPos) const;
};