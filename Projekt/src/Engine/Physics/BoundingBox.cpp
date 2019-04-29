#include "BoundingBox.h"

BoundingBox::BoundingBox() {
	m_modelMatrix = glm::mat4();
	m_minDistances = glm::vec3(-1.0f, -1.0f, -1.0f);
	m_maxDistances = glm::vec3(1.0f, 1.0f, 1.0f);
}

BoundingBox::~BoundingBox() {

}

void BoundingBox::createBoundingBox(glm::vec3 minDistances, glm::vec3 maxDistances) {
	m_minDistances = minDistances;
	m_maxDistances = maxDistances;

	//----m_boundingBox----
	m_boundingBox.walls.clear();
	m_boundingBox.origin = glm::vec3(0.0f, 0.0f, 0.0f);

	Plane tempPlane;
	//Left plane
	tempPlane.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	tempPlane.distance = -m_minDistances.x;
	m_boundingBox.walls.push_back(tempPlane);

	//Right plane
	tempPlane.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	tempPlane.distance = m_maxDistances.x;
	m_boundingBox.walls.push_back(tempPlane);

	//Near plane
	tempPlane.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	tempPlane.distance = m_maxDistances.z;
	m_boundingBox.walls.push_back(tempPlane);

	//Far plane
	tempPlane.normal = glm::vec3(0.0f, 0.0f, -1.0f);
	tempPlane.distance = -m_minDistances.z;
	m_boundingBox.walls.push_back(tempPlane);

	//Top plane
	tempPlane.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	tempPlane.distance = m_maxDistances.y;
	m_boundingBox.walls.push_back(tempPlane);

	//Bottom plane
	tempPlane.normal = glm::vec3(0.0f, -1.0f, 0.0f);
	tempPlane.distance = -m_minDistances.y;
	m_boundingBox.walls.push_back(tempPlane);

	m_boundingBox.origin = glm::vec3(0.0f, 0.0f, 0.0f);
	//---------------------
}

void BoundingBox::setModelMatrix(glm::mat4 modelMatrix) {
	m_modelMatrix = modelMatrix;
}

void BoundingBox::recalculateBoundingBox() {
	//----m_boundingBox----
	m_boundingBox.walls.clear();
	m_boundingBox.origin = glm::vec3(0.0f, 0.0f, 0.0f);

	m_boundingBox.origin = glm::vec3(m_modelMatrix * glm::vec4(m_boundingBox.origin, 1.0f));

	Plane tempPlane;
	glm::vec3 tempVec;
	//Left plane
	tempVec = glm::vec3(m_minDistances.x, 0.0f, 0.0f);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);

	//Right plane
	tempVec = glm::vec3(m_maxDistances.x, 0.0f, 0.0f);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);

	//Near plane
	tempVec = glm::vec3(0.0f, 0.0f, m_maxDistances.z);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);

	//Far plane
	tempVec = glm::vec3(0.0f, 0.0f, m_minDistances.z);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);

	//Top plane
	tempVec = glm::vec3(0.0f, m_maxDistances.y, 0.0f);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);

	//Bottom plane
	tempVec = glm::vec3(0.0f, m_minDistances.y, 0.0f);
	tempVec = glm::vec3(m_modelMatrix * glm::vec4(tempVec, 1.0f)) - m_boundingBox.origin;
	tempPlane.distance = glm::length(tempVec);
	tempPlane.normal = glm::normalize(tempVec);
	m_boundingBox.walls.push_back(tempPlane);
	//---------------------
}

glm::vec3 BoundingBox::findCorner(glm::vec3 origin, Plane wall1, Plane wall2, Plane wall3) {
	glm::vec3 returnVector;
	returnVector = origin + wall1.normal * wall1.distance + wall2.normal * wall2.distance + wall3.normal * wall3.distance;
	return returnVector;
}

glm::vec3 BoundingBox::triangleIntersection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
	//This is really ugly. Implement a proper triangle to box test instead. Or atleast create and transform actual triangles in this class.

	glm::vec3 returnVector = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 triangleMiddle = (p0 + p1 + p2) / 3.0f;
	glm::vec3 triangleNormal = glm::normalize(glm::cross(p1 - p0, p2 - p1));

	//Make the triangle a little smaller (to avoid false positives when a bounding box is colliding with a pointy corner of a mesh for example)
	p0 = p0 - (p0 - triangleMiddle) * 0.05f;
	p1 = p1 - (p1 - triangleMiddle) * 0.05f;
	p2 = p2 - (p2 - triangleMiddle) * 0.05f;

	//Get corners of static object
	std::vector<glm::vec3> boxCorners;
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[0], m_boundingBox.walls[2], m_boundingBox.walls[4])); //Top close left - 0
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[1], m_boundingBox.walls[2], m_boundingBox.walls[4])); //Top close right - 1
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[0], m_boundingBox.walls[3], m_boundingBox.walls[4])); //Top far left - 2
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[1], m_boundingBox.walls[3], m_boundingBox.walls[4])); //Top far right - 3
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[0], m_boundingBox.walls[2], m_boundingBox.walls[5])); //Bottom close left - 4
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[1], m_boundingBox.walls[2], m_boundingBox.walls[5])); //Bottom close right - 5
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[0], m_boundingBox.walls[3], m_boundingBox.walls[5])); //Bottom far left - 6
	boxCorners.push_back(findCorner(m_boundingBox.origin, m_boundingBox.walls[1], m_boundingBox.walls[3], m_boundingBox.walls[5])); //Bottom far right - 7

	/*

	  2--------3
	 /|       /|
	/0------1/ |
	| |     |  |
	| /6----|-7/
	|/      | /
	|4-----5|/
	*/


	int indices[36] = {
		// front
		0, 4, 1,
		1, 4, 5,
		// right
		1, 5, 3,
		3, 5, 7,
		// back
		3, 7, 2,
		2, 7, 6,
		// left
		2, 6, 0,
		0, 6, 4,
		// bottom
		4, 6, 5,
		5, 6, 7,
		// top
		2, 0, 3,
		3, 0, 1
	};

	glm::vec3 triangle1[3] = { p0, p1, p2 };

	for (int i = 0; i < 36; i += 3) {
		glm::vec3 triangle2[3] = { boxCorners[indices[i]], boxCorners[indices[i + 1]], boxCorners[indices[i + 2]] };
		if (Intersections::triTriTest(triangle1, triangle2) == true) {
			return triangleNormal;
		}
	}

	return returnVector;
}
