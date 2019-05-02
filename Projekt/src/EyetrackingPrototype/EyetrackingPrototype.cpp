#include "EyetrackingPrototype.h"
#include "SFML/Window.hpp"
#include <iostream>

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_FOV = 80.0;
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

	glm::vec3 mousePos;
	glm::vec2 screenSize;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		std::cout << "Break \n";
	}

	mousePos.x = sf::Mouse::getPosition(*m_window).x;
	mousePos.y = sf::Mouse::getPosition(*m_window).y;
	mousePos.z = 0.f;

	glm::vec3 mouseRay;
	glm::vec2 angle;
	screenSize.x = m_window->getSize().x;
	screenSize.y = m_window->getSize().y;

	angle.x = mousePos.x - (screenSize.x / 2.f);
	angle.y = mousePos.y - (screenSize.y / 2.f);

	angle.x = (angle.x / screenSize.x) * m_FOV * (screenSize.x / screenSize.y);
	angle.y = (angle.y / screenSize.y) * m_FOV;

	glm::vec3 cameraDir = m_camera->getDirection();
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), cameraDir));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight));

	mouseRay = glm::rotate(cameraDir, -glm::radians(angle.x), cameraUp);
	mouseRay = glm::rotate(mouseRay, glm::radians(angle.y), cameraRight);

	

	glm::vec2 pos = m_heightmap->calculateMousePos(mouseRay, m_camera->getPosition());


	return pos;
}
