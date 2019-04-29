#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Intersections.h"

class BoundingBox {
private:
	struct Plane {
		glm::vec3 normal;
		float distance;
	};

	struct BoxWalls {
		std::vector<Plane> walls;
		glm::vec3 origin;
	};

	BoxWalls m_boundingBox;
	glm::vec3 m_minDistances;
	glm::vec3 m_maxDistances;

	glm::mat4 m_modelMatrix;

	glm::vec3 findCorner(glm::vec3 origin, Plane wall1, Plane wall2, Plane wall3);
public:
	BoundingBox();
	~BoundingBox();

	void createBoundingBox(glm::vec3 minDistances, glm::vec3 maxDistances);
	void setModelMatrix(glm::mat4 modelMatrix);
	void recalculateBoundingBox();

	glm::vec3 triangleIntersection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
};