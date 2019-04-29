#pragma once

#include <glm\glm.hpp>

#include "..\Graphics\Geometry\Model loading\ModelHandler.h"
#include "..\Graphics\Shader compilation\ShaderSet.h"
#include "..\Physics\BoundingBox.h"
#include "..\Graphics\Lighting\PointLight.h"
#include "..\Graphics\Geometry\Predefined structures\Plane.h"
#include "..\Graphics\Geometry\Predefined structures\MeshBuilder.h"

class Object {
private:
	ModelHandler* m_modelHandler;

	//Flags
	bool m_modelIsActive;
	bool m_recalculateBoundingBox;
	bool m_isLit;
	bool m_planeIsActive;
	bool m_meshBuilderIsActive;

	//Components
	Model* m_model;
	BoundingBox m_boundingBox;
	PointLight m_pointLight;
	Plane m_plane;
	MeshBuilder m_meshBuilder;

	//Properties
	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_rotation;
	glm::vec3 m_rotationAxis;
	glm::mat4 m_modelMatrix;
	bool m_modelMatrixHasChanged;

	//----Member functions----
	void updateModelMatrix();
	//------------------------
public:
	//----Public functions----
	Object(ModelHandler* modelHandler);
	~Object();

	//Flag setters
	void setModelVisable(bool state);
	void setBoundingBoxActive(bool state);
	void setPointLightActive(bool state);
	void setPlaneActive(bool state);
	void setMeshBuilderActive(bool state);

	//Data setters
	void setModel(std::string modelPath);
	void setBoundingBox(glm::vec3 minCorner, glm::vec3 maxCorner);
	void useModelBoundingBox();

	//Property setters
	void setPosition(glm::vec3 position);
	void setScale(glm::vec3 scale);
	void setRotation(float rotationAmount, glm::vec3 rotationAxis);
	void setModelMatrix(glm::mat4 modelMatrix);

	//Flag getters
	bool isLit() const;

	//Property getters
	glm::vec3 getPosition() const;
	float getRotation() const;
	glm::vec3 getRotationAxis() const;

	//Component getters
	PointLight* getPointLight();
	Plane* getPlane();
	MeshBuilder* getMeshBuilder();
	BoundingBox* getBoundingBox();

	//Updates
	void update(float dt);

	//Draw
	void draw(GLuint shaderProgram);
	//------------------------
};