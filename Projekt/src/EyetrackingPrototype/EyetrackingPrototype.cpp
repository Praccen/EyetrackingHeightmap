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

void EyetrackingPrototype::controlCamera(float dt) {
	glm::vec3 newCamDir = m_camera->getDirection();
	float speed = 10.0f;
	
	// UP v2
	/*if (cursor->getPos().y < 450.0f) {
		speed = 10.0f * ((450.0f - cursor->getPos().y) / 450.0f);
		m_camera->setPosition(m_camera->getPosition() + glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z)) * speed * dt);
	}*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		m_camera->setPosition(m_camera->getPosition() + glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z)) * speed * dt);
	}

	// DOWN v2
	/*if (cursor->getPos().y > 450.0f) {
	speed = 10.0f * ((900.0f - cursor->getPos().y) / 450.0f);
	m_camera->setPosition(m_camera->getPosition() - glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z)) * speed * dt);
	}*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		m_camera->setPosition(m_camera->getPosition() - glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z)) * speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		m_camera->setPosition(m_camera->getPosition() + glm::normalize(glm::cross(m_camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		m_camera->setPosition(m_camera->getPosition() - glm::normalize(glm::cross(m_camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
		m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		m_camera->setPosition(m_camera->getPosition() - glm::vec3(0.0f, 1.0f, 0.0f) * speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && m_camera->getDirection().y < 0.95) {
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), 1.57f * dt, glm::cross(m_camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && m_camera->getDirection().y > -0.95) {
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), -1.57f * dt, glm::cross(m_camera->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f))));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), -1.57f * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), 1.57f * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
}

void EyetrackingPrototype::update(float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
		std::cout << getMousePos().x << ", " << getMousePos().y << std::endl;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->paintGround(tempMousePos, 1, 2);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->paintGround(tempMousePos, 2, 2);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->paintGround(tempMousePos, 3, 2);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->paintGround(tempMousePos, 4, 2);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->paintGround(tempMousePos, 0, 2);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->lowerGround(tempMousePos, 3 * dt, 5);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
		glm::vec2 tempMousePos = getMousePos();
		if (tempMousePos.x != -1 && tempMousePos.y != -1) {
			m_heightmap->raiseGround(tempMousePos, 3 * dt, 5);
		}
	}

	controlCamera(dt);
}
