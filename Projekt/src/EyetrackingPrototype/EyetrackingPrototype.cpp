#include "EyetrackingPrototype.h"
#include "SFML/Window.hpp"
#include <iostream>
#include <tobii/tobii.h>

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_FOV = 45.0f;
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

	m_timer = 1.0f;
	m_lastMousePick = glm::vec2(0.0f, 0.0f);
}

EyetrackingPrototype::~EyetrackingPrototype() {
	delete m_heightmap;
}

glm::vec2 EyetrackingPrototype::getMousePos() {
	if (m_timer > 0.1) {
		m_timer = 0;

		//Calculate normalized device coordinates
		glm::vec2 ndc;
		/*ndc.x = ((2.f * sf::Mouse::getPosition(*m_window).x) / m_window->getSize().x) - 1.f;
		ndc.y = 1.f - ((2.f * sf::Mouse::getPosition(*m_window).y) / m_window->getSize().y);*/

		ndc.x = 2.f * m_eyePos.x - 1.f;
		ndc.y = 1.f - 2.f * m_eyePos.y;

		//Calculate normalized mouse ray in world space
		glm::vec4 mouseRayClip = glm::vec4(ndc.x, ndc.y, -1.f, 1.f);
		glm::vec4 mouseRayCamera = glm::inverse(m_camera->getProjection()) * mouseRayClip;
		mouseRayCamera = glm::vec4(mouseRayCamera.x, mouseRayCamera.y, -1.f, 0.f);
		glm::vec3 mouseRayWorld = glm::normalize(glm::vec3(glm::inverse(m_camera->getView()) * mouseRayCamera));

		//Get pos on heightmap
		m_lastMousePick = m_heightmap->calculateMousePos(mouseRayWorld, m_camera->getPosition());
	}

	return m_lastMousePick;
}

void EyetrackingPrototype::controlCamera(float dt) {
	glm::vec3 newCamDir = m_camera->getDirection();
	float speed = 40.0f;

	/*float x_cursor = sf::Mouse::getPosition(*m_window).x;
	float y_cursor = sf::Mouse::getPosition(*m_window).y;*/

	float x_cursor = m_eyePos.x * m_window->getSize().x;
	float y_cursor = m_eyePos.y * m_window->getSize().y;

	// ----- PANNING -----
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		m_camera->setPosition(m_camera->getPosition() - glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z)) * speed * dt * (y_cursor - m_window->getSize().y / 2.0f) / (float)m_window->getSize().y);
		m_camera->setPosition(m_camera->getPosition() - glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0), glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z))) * speed * dt * (x_cursor - m_window->getSize().x / 2.0f) / (float)m_window->getSize().x);
	}

	// ----- ZOOM -----
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * -speed * dt * (y_cursor - m_window->getSize().y / 2.0f) / (float)m_window->getSize().y);
	}

	// ----- ROTATION -----
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), 1.5f * dt * 0.5f * ((y_cursor - (m_window->getSize().y / 2)) / (m_window->getSize().y / 2)), glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_camera->getDirection())));
		m_camera->setDirection(glm::rotate(m_camera->getDirection(), 1.5f * dt * -0.5f * ((x_cursor - (m_window->getSize().x / 2)) / (m_window->getSize().x / 2)), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
}

void EyetrackingPrototype::setEyePos(glm::vec2 eyePos) {
	m_eyePos = eyePos;
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
			m_heightmap->paintGround(tempMousePos, 4, 5);
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

	sf::Mouse::setPosition(sf::Vector2i(m_eyePos.x * m_window->getSize().x, m_eyePos.y * m_window->getSize().y), *m_window);

	m_timer += dt;
}
