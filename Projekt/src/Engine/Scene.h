#pragma once

#include <GL\glew.h>
#include <vector>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include "Content\Object.h"
#include "Graphics/Geometry/GUI Element/GUIElement.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Lighting/DirectionalLight.h"

class Scene {
private:
	ModelHandler m_modelHandler;
	std::vector<Object*> m_objects;
	std::vector<GUIElement*> m_GUIElements;

	Camera m_camera;
	DirectionalLight m_directionalLight;

	int m_nrOfPointLights;
	bool m_removeGUIElement = false;

	//Geometry shader locations
	GLint m_projLoc, m_viewLoc, m_geometryCameraPosLoc;

	//Lighting shader locations
	GLint m_lightCameraPosLoc, m_nrOfPointLightsLoc;

public:
	Scene();
	~Scene();

	Object* addObject();
	GUIElement* addGUIElement();
	Camera* getCamera();
	DirectionalLight* getDirectionalLight();

	int getNrOfPointLights();

	void setGeometryUniforms(GLuint shaderProgram);
	void setShadowUniforms(GLuint shaderProgram);
	void setLightingUniforms(GLuint shaderProgram);
	void update(float dt);
	void draw3D(GLuint shaderProgram);
	void drawGUI(GLuint shaderProgram);
};