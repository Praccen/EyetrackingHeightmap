#include "EyetrackingPrototype.h"

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_camera->setFOV(65.0);
	m_camera->setPosition(glm::vec3(50.0f, 40.0f, -20.0f));
	m_camera->setDirection(glm::vec3(0.0f, -1.0f, 1.0f));

	//Directional light
	m_directionalLight = Application::getDirectionalLight();
	m_directionalLight->setColour(glm::vec3(1.0f));
	m_directionalLight->setDirection(glm::vec3(0.0f, -1.0f, 1.1f));
	m_directionalLight->setAmbientMultiplier(0.15f);

	m_heightmap = new Heightmap();
}

EyetrackingPrototype::~EyetrackingPrototype() {
	delete m_heightmap;
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
	controlCamera(dt);
}
