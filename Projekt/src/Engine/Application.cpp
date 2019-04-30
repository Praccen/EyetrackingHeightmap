#include "Application.h"

Scene Application::m_scene;


Application::Application(sf::Window *originalWindow) {
	m_deferredRenderer = new DeferredRenderer(originalWindow, &m_scene);
}

Application::~Application() {
	delete m_deferredRenderer;
}

void Application::updateEngine(float dt) {
	m_scene.update(dt);
}

void Application::draw() {
	m_deferredRenderer->draw();
}

//----Static member functions to get pointers to engine content----
Object* Application::getNewObject() {
	return m_scene.addObject();
}

Camera* Application::getCamera() {
	return m_scene.getCamera();
}

DirectionalLight* Application::getDirectionalLight() {
	return m_scene.getDirectionalLight();
}
ShaderSet * Application::getMenuShader() const {
	return m_deferredRenderer->getMenuPointer();
}
//-----------------------------------------------------------------