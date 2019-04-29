#pragma once

#include <glm/glm.hpp>
#include <algorithm>

class Intersections {
private:
	// The first input is the plane (determined by triangle U) and the
	// second input is triangle.
	static bool triTriSupport(glm::vec3 U[3], glm::vec3 V[3], glm::vec3 segment[2]);

public:
	Intersections();
	~Intersections();

	static float rayTriTest(glm::vec3 rayStart, glm::vec3 rayVec, glm::vec3 triangleCorners[3]);
	static bool triTriTest(glm::vec3 triangle1[3], glm::vec3 triangle2[3]);
};