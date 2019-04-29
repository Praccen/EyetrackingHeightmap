#include "EyetrackingPrototype.h"

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_camera->setFOV(65.0);
	m_camera->setPosition(glm::vec3(50.0f, 30.0f, 65.0f));
	m_camera->setDirection(glm::vec3(0.0f, -1.0f, -0.5f));

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

void EyetrackingPrototype::readKeys() {
	glm::vec3 newCamPos = m_camera->getPosition();
	// ZOOM
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		m_camera->setPosition(m_camera->getPosition() + m_camera->getDirection());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
		m_camera->setPosition(m_camera->getPosition() - m_camera->getDirection());
	}
	// MOVE
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		newCamPos.z -= 1.0f;
		m_camera->setPosition(newCamPos);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		newCamPos.z += 1.0f;
		m_camera->setPosition(newCamPos);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		newCamPos.x -= 1.0f;
		m_camera->setPosition(newCamPos);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		newCamPos.x += 1.0f;
		m_camera->setPosition(newCamPos);
	}
	// ROTATE
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {

	}

	// PRINT current pos
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		std::cout << "X: " << m_camera->getPosition().x << "   Y:" << m_camera->getPosition().y << "   Z:" << m_camera->getPosition().z << std::endl;
	}
}

void EyetrackingPrototype::update(float dt) {
	readKeys();
}
