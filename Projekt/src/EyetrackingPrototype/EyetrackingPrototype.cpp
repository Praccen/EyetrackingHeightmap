#include "EyetrackingPrototype.h"
#include "SFML/Window.hpp"
#include <iostream>

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_FOV = 80.0f;
	m_camera->setFOV(m_FOV);
	m_camera->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	m_camera->setDirection(glm::vec3(1.0f, -0.5f, 1.0f));

	//Directional light
	m_directionalLight = Application::getDirectionalLight();
	m_directionalLight->setColour(glm::vec3(1.0f));
	m_directionalLight->setDirection(glm::vec3(0.0f, -1.0f, 1.1f));
	m_directionalLight->setAmbientMultiplier(0.15f);

	m_heightmap = new Heightmap();

	m_window = originalWindow;
}

EyetrackingPrototype::~EyetrackingPrototype() {
	delete m_heightmap;
}

void EyetrackingPrototype::update(float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
		std::cout << getMousePos().x << ", " << getMousePos().y << std::endl;
	}
}

glm::vec2 EyetrackingPrototype::getMousePos() {
	//Calculate normalized device coordinates
	glm::vec2 ndc;
	ndc.x = ((2.f * sf::Mouse::getPosition(*m_window).x) / m_window->getSize().x) - 1.f;
	ndc.y = 1.f - ((2.f * sf::Mouse::getPosition(*m_window).y) / m_window->getSize().y);
	
	//Calculate normalized mouse ray in world space
	glm::vec4 mouseRayClip = glm::vec4(ndc.x, ndc.y, -1.f, 1.f);
	glm::vec4 mouseRayCamera = glm::inverse(m_camera->getProjection()) * mouseRayClip;
	mouseRayCamera = glm::vec4(mouseRayCamera.x, mouseRayCamera.y, -1.f, 0.f);
	glm::vec3 mouseRayWorld = glm::normalize(glm::vec3(glm::inverse(m_camera->getView()) * mouseRayCamera));
	
	//Get pos on heightmap
	glm::vec2 pos = m_heightmap->calculateMousePos(mouseRayWorld, m_camera->getPosition());


	return pos;
}
