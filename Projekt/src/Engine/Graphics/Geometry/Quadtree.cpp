#include "Quadtree.h"

Quadtree::Quadtree() {
	/*fixedHeight = 1000.0f;
	root = nullptr;*/
}

Quadtree::~Quadtree() {
	/*if (root != nullptr)
		delete root;*/
}

//BoundingBox Quadtree::buildBoundingBox(float boxWidth, float boxLength, glm::vec3 boxCenter) {
//	BoundingBox tempBox;
//	tempBox.origin = boxCenter;
//	tempBox.width = boxWidth;
//	tempBox.length = boxLength;
//
//	Plane tempPlane;
//	//Left plane
//	tempPlane.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
//	tempPlane.distance = tempBox.width / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	//Right plane
//	tempPlane.normal = glm::vec3(1.0f, 0.0f, 0.0f);
//	tempPlane.distance = tempBox.width / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	//Near plane
//	tempPlane.normal = glm::vec3(0.0f, 0.0f, 1.0f);
//	tempPlane.distance = tempBox.length / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	//Far plane
//	tempPlane.normal = glm::vec3(0.0f, 0.0f, -1.0f);
//	tempPlane.distance = tempBox.length / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	//Top plane
//	tempPlane.normal = glm::vec3(0.0f, 1.0f, 0.0f);
//	tempPlane.distance = fixedHeight / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	//Bottom plane
//	tempPlane.normal = glm::vec3(0.0f, -1.0f, 0.0f);
//	tempPlane.distance = fixedHeight / 2.0f;
//	tempBox.walls.push_back(tempPlane);
//
//	return tempBox;
//}
//
//glm::vec3 Quadtree::findCorner(glm::vec3 origin, Plane wall1, Plane wall2, Plane wall3) {
//	glm::vec3 returnVector;
//	returnVector = origin + wall1.normal * wall1.distance + wall2.normal * wall2.distance + wall3.normal * wall3.distance;
//	return returnVector;
//}
//
//bool Quadtree::boundingBoxesTest(BoundingBox bb1, BoundingBox bb2) {
//	bool returnValue = true;
//
//	//Get corners of box 1
//	std::vector<glm::vec3> corners;
//	corners.push_back(findCorner(bb1.origin, bb1.walls[0], bb1.walls[2], bb1.walls[4])); //Top close left
//	corners.push_back(findCorner(bb1.origin, bb1.walls[1], bb1.walls[2], bb1.walls[4])); //Top close right
//	corners.push_back(findCorner(bb1.origin, bb1.walls[0], bb1.walls[3], bb1.walls[4])); //Top far left
//	corners.push_back(findCorner(bb1.origin, bb1.walls[1], bb1.walls[3], bb1.walls[4])); //Top far right
//	corners.push_back(findCorner(bb1.origin, bb1.walls[0], bb1.walls[2], bb1.walls[5])); //Bottom close left
//	corners.push_back(findCorner(bb1.origin, bb1.walls[1], bb1.walls[2], bb1.walls[5])); //Bottom close right
//	corners.push_back(findCorner(bb1.origin, bb1.walls[0], bb1.walls[3], bb1.walls[5])); //Bottom far left
//	corners.push_back(findCorner(bb1.origin, bb1.walls[1], bb1.walls[3], bb1.walls[5])); //Bottom far right
//
//	//Get diagonals in box 1
//	std::vector<glm::vec3> diagonals;
//	diagonals.push_back(glm::normalize(corners[0] - corners[7]));
//	diagonals.push_back(glm::normalize(corners[1] - corners[6]));
//	diagonals.push_back(glm::normalize(corners[2] - corners[5]));
//	diagonals.push_back(glm::normalize(corners[3] - corners[4]));
//
//	float max;
//	glm::vec3 positivePoint;
//	glm::vec3 negativePoint;
//
//	//Loop through all planes
//	for (unsigned int i = 0; i < bb2.walls.size(); i++) {
//		if (returnValue == true) { //Stop calculating if we have been outside any plane
//			max = -1.0f;
//			//Check diagonals with normals
//			for (unsigned int j = 0; j < diagonals.size(); j++) {
//				float temp = glm::dot(diagonals[j], glm::normalize(bb2.walls[i].normal));
//				if (temp > max) {
//					max = temp;
//					positivePoint = corners[j];
//					negativePoint = corners[7 - j];
//				}
//
//				temp = glm::dot(-diagonals[j], glm::normalize(bb2.walls[i].normal));
//				if (temp > max) {
//					max = temp;
//					positivePoint = corners[7 - j];
//					negativePoint = corners[j];
//				}
//			}
//			//Check which side of the plane the points are
//			float distancePositivePoint = glm::dot(positivePoint - bb2.origin, glm::normalize(bb2.walls[i].normal)) - bb2.walls[i].distance;
//			float distanceNegativePoint = glm::dot(negativePoint - bb2.origin, glm::normalize(bb2.walls[i].normal)) - bb2.walls[i].distance;
//
//			if (distanceNegativePoint > 0.0f) {
//				returnValue = false;
//			}
//		}
//	}
//
//	return returnValue;
//}
//
//bool Quadtree::frustumTest(BoxCorners frustum, BoundingBox bb) {
//	bool returnValue = true;
//
//	//Get diagonals in frustum
//	std::vector<glm::vec3> diagonals;
//	diagonals.push_back(glm::normalize(frustum.corners[0] - frustum.corners[7]));
//	diagonals.push_back(glm::normalize(frustum.corners[1] - frustum.corners[6]));
//	diagonals.push_back(glm::normalize(frustum.corners[2] - frustum.corners[5]));
//	diagonals.push_back(glm::normalize(frustum.corners[3] - frustum.corners[4]));
//
//	float max;
//	glm::vec3 positivePoint;
//	glm::vec3 negativePoint;
//
//	//Loop through all planes
//	for (unsigned int i = 0; i < bb.walls.size(); i++) {
//		if (returnValue == true) { //Stop calculating if we have been outside any plane
//			max = -1.0f;
//			//Check diagonals with normals
//			for (unsigned int j = 0; j < diagonals.size(); j++) {
//				float temp = glm::dot(diagonals[j], glm::normalize(bb.walls[i].normal));
//				if (temp > max) {
//					max = temp;
//					positivePoint = frustum.corners[j];
//					negativePoint = frustum.corners[7 - j];
//				}
//
//				temp = glm::dot(-diagonals[j], glm::normalize(bb.walls[i].normal));
//				if (temp > max) {
//					max = temp;
//					positivePoint = frustum.corners[7 - j];
//					negativePoint = frustum.corners[j];
//				}
//			}
//			//Check which side of the plane the points are
//			float distancePositivePoint = glm::dot(positivePoint - bb.origin, glm::normalize(bb.walls[i].normal)) - bb.walls[i].distance;
//			float distanceNegativePoint = glm::dot(negativePoint - bb.origin, glm::normalize(bb.walls[i].normal)) - bb.walls[i].distance;
//
//			if (distanceNegativePoint > 0.0f) {
//				returnValue = false;
//			}
//		}
//	}
//
//	return returnValue;
//}
//
//void Quadtree::buildTree(std::vector<Model> models, float sceneWidth, float sceneLength, glm::vec3 sceneCenter) {
//	root = new Node();
//	root->models = models;
//	root->boundingBox = buildBoundingBox(sceneWidth, sceneLength, sceneCenter);
//	root->depth = 1;
//	recursiveBuild(*root, root->depth);
//}
//
//void Quadtree::recursiveBuild(Node &node, int depth) {
//	depth++; //Increase depth counter
//
//	//Divide bounding box
//	std::vector<BoundingBox> boundingBoxes;
//	boundingBoxes.push_back(buildBoundingBox(node.boundingBox.width / 2.0f, node.boundingBox.length / 2.0f, glm::vec3(node.boundingBox.origin.x - node.boundingBox.width / 4.0f, node.boundingBox.origin.y, node.boundingBox.origin.z - node.boundingBox.length / 4.0f)));
//	boundingBoxes.push_back(buildBoundingBox(node.boundingBox.width / 2.0f, node.boundingBox.length / 2.0f, glm::vec3(node.boundingBox.origin.x + node.boundingBox.width / 4.0f, node.boundingBox.origin.y, node.boundingBox.origin.z - node.boundingBox.length / 4.0f)));
//	boundingBoxes.push_back(buildBoundingBox(node.boundingBox.width / 2.0f, node.boundingBox.length / 2.0f, glm::vec3(node.boundingBox.origin.x - node.boundingBox.width / 4.0f, node.boundingBox.origin.y, node.boundingBox.origin.z + node.boundingBox.length / 4.0f)));
//	boundingBoxes.push_back(buildBoundingBox(node.boundingBox.width / 2.0f, node.boundingBox.length / 2.0f, glm::vec3(node.boundingBox.origin.x + node.boundingBox.width / 4.0f, node.boundingBox.origin.y, node.boundingBox.origin.z + node.boundingBox.length / 4.0f)));
//
//	//Create childnodes
//	for (int i = 0; i < 4; i++) {
//		Node tempNode;
//		tempNode.boundingBox = boundingBoxes[i];
//		tempNode.depth = depth;
//		node.childNodes.push_back(tempNode);
//	}
//
//	//Check against models
//	for (unsigned int i = 0; i < node.models.size(); i++) {
//		for (int j = 0; j < 4; j++) {
//			if (boundingBoxesTest(node.models[i].getBoundingBox(), node.childNodes[j].boundingBox)) {
//				node.childNodes[j].models.push_back(node.models[i]);
//			}
//		}
//	}
//
//	//Remove models from parent node
//	node.models.clear();
//
//	//Call function for each of the new nodes
//	for (int i = 0; i < 4; i++) {
//		if (node.childNodes[i].models.size() > 0 && depth < 4) {
//			recursiveBuild(node.childNodes[i], depth);
//		}
//	}
//}
//
//void Quadtree::recursiveDraw(Node &node, BoxCorners frustum, GLuint shaderProgram) {
//	if (frustumTest(frustum, node.boundingBox)) {
//		if (node.childNodes.size() == 0) { //Leaf node - draw all models inside
//			for (unsigned int i = 0; i < node.models.size(); i++) {
//				node.models[i].draw(shaderProgram);
//			}
//		}
//		else { //Not a leaf node - run function for all child nodes
//			for (unsigned int i = 0; i < node.childNodes.size(); i++) {
//				recursiveDraw(node.childNodes[i], frustum, shaderProgram);
//			}
//		}
//	}
//}
//
//void Quadtree::drawVisible(BoxCorners frustum, GLuint shaderProgram) {
//	recursiveDraw(*root, frustum, shaderProgram);
//}