#pragma once

#include <GL\glew.h>
#include <vector>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include "Content\Object.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Lighting/DirectionalLight.h"

class Scene {
private:
	ModelHandler m_modelHandler;
	std::vector<Object*> m_objects;

	Camera m_camera;
	DirectionalLight m_directionalLight;

	int m_nrOfPointLights;

	//Geometry shader locations
	GLint m_projLoc, m_viewLoc, m_geometryCameraPosLoc;

	//Lighting shader locations
	GLint m_lightCameraPosLoc, m_nrOfPointLightsLoc;

public:
	Scene();
	~Scene();

	Object* addObject();
	Camera* getCamera();
	DirectionalLight* getDirectionalLight();

	int getNrOfPointLights();

	void setGeometryUniforms(GLuint shaderProgram);
	void setShadowUniforms(GLuint shaderProgram);
	void setLightingUniforms(GLuint shaderProgram);
	void update(float dt);
	void draw(GLuint shaderProgram);
};