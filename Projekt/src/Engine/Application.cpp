#include "Application.h"

Scene Application::m_scene;


Application::Application(sf::Window *originalWindow) {
	m_deferredRenderer = new DeferredRenderer(originalWindow, &m_scene);
	m_GUIRenderer = new GUIRenderer(originalWindow, &m_scene);
}

Application::~Application() {
	delete m_deferredRenderer;
	delete m_GUIRenderer;
}

void Application::updateEngine(float dt) {
	m_scene.update(dt);
}

void Application::draw() {
	m_deferredRenderer->draw();
	m_GUIRenderer->draw();
}

//----Static member functions to get pointers to engine content----
Object* Application::getNewObject() {
	return m_scene.addObject();
}

GUIElement* Application::getNewUIElement() {
	return m_scene.addGUIElement();
}

Camera* Application::getCamera() {
	return m_scene.getCamera();
}

DirectionalLight* Application::getDirectionalLight() {
	return m_scene.getDirectionalLight();
}
//-----------------------------------------------------------------