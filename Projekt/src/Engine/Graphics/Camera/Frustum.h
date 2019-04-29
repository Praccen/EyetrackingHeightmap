#ifndef TESTFRUSTUM_H
#define TESTFRUSTUM_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

//#include "..\Geometry\Structs.h"

class Frustum {
private:
	glm::mat4 proj, mv, temp, inv;
	std::vector<glm::vec4> points;
	//BoxCorners corners;

public:
	Frustum();
	~Frustum();

	//BoxCorners getCorners();

	void update(glm::mat4 projection, glm::mat4 modelview);
};


#endif