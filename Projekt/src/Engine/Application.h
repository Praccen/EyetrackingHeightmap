#pragma once

#include <SFML\Window.hpp>

#include <glm/gtx/rotate_vector.hpp>

#include "Graphics\Renderers\DeferredRenderer.h"
#include "Graphics\Camera\Camera.h"
#include "Graphics\Lighting\DirectionalLight.h"
#include "Scene.h"

class Application {
private:
	DeferredRenderer * m_deferredRenderer;
	static Scene m_scene;

public:
	Application(sf::Window *originalWindow);
	virtual ~Application();

	static Object* getNewObject();
	static Camera* getCamera();
	static DirectionalLight* getDirectionalLight();

	void updateEngine(float dt);
	void draw();

//protected:
	
};