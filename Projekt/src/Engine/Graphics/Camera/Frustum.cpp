#include "Frustum.h"

Frustum::Frustum() {
	points.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f)); //Top close left
	points.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); //Top close right
	points.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f)); //Top far left
	points.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f)); //Top far right
	points.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f)); //Bottom close left
	points.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f)); //Bottom close right
	points.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f)); //Bottom far left
	points.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f)); //Bottom far right

	/*for (int i = 0; i < 8; i++) {
		corners.corners.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}*/
}

Frustum::~Frustum() {

}

//BoxCorners Frustum::getCorners() {
//	return corners;
//}

void Frustum::update(glm::mat4 projection, glm::mat4 modelview) {
	/*proj = projection;
	mv = modelview;
	temp = proj * mv;
	inv = glm::inverse(temp);

	corners.corners.clear();

	for (int i = 0; i < 8; i++) {
		glm::vec4 tempVec4;
		glm::vec3 tempVec3;
		tempVec4 = inv * points[i];
		tempVec3.x = tempVec4.x / tempVec4.w;
		tempVec3.y = tempVec4.y / tempVec4.w;
		tempVec3.z = tempVec4.z / tempVec4.w;
		corners.corners.push_back(tempVec3);
	}*/
}