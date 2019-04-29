#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

#include "..\Geometry\Model loading\Model.h"
#include "..\Shader compilation\ShaderSet.h"
//#include "..\Geometry\Structs.h"

class Quadtree {
private:
	/*struct Node {
		std::vector<Node> childNodes;
		std::vector<Model> models;
		BoundingBox boundingBox;
		int depth;
	};

	float fixedHeight;

	void recursiveBuild(Node &node, int depth);
	void recursiveDraw(Node &node, BoxCorners frustum, GLuint shaderProgram);
	BoundingBox buildBoundingBox(float boxWidth, float boxLength, glm::vec3 boxCenter);
	glm::vec3 findCorner(glm::vec3 origin, Plane wall1, Plane wall2, Plane wall3);

	Node *root;*/
public:
	Quadtree();
	~Quadtree();
	/*bool boundingBoxesTest(BoundingBox bb1, BoundingBox bb2);
	bool frustumTest(BoxCorners frustum, BoundingBox bb);
	void buildTree(std::vector<Model> models, float sceneWidth, float sceneLength, glm::vec3 sceneCenter);
	void drawVisible(BoxCorners frustum, GLuint shaderProgram);*/
};

#endif