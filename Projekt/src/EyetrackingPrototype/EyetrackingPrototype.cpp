#include "EyetrackingPrototype.h"

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_camera->setFOV(65.0);
	m_camera->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	m_camera->setDirection(glm::vec3(1.0f, -1.0f, 1.0f));

	//Directional light
	m_directionalLight = Application::getDirectionalLight();
	m_directionalLight->setColour(glm::vec3(1.0f));
	m_directionalLight->setDirection(glm::vec3(0.0f, -1.0f, 1.1f));
	m_directionalLight->setAmbientMultiplier(0.15f);

	m_position = glm::vec2(0.0, 0.0);

	m_heightmap = new Heightmap();
}

EyetrackingPrototype::~EyetrackingPrototype() {
	delete m_heightmap;
}

void EyetrackingPrototype::update(float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		m_position.x += 10.0 * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		m_position.x -= 10.0 * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		m_position.y += 10.0 * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		m_position.y -= 10.0 * dt;
	}

	m_position.x = std::min(std::max(m_position.x, 0.0f), 100.0f);
	m_position.y = std::min(std::max(m_position.y, 0.0f), 100.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) {
		m_heightmap->paintGround(m_position, 0, 4.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
		m_heightmap->paintGround(m_position, 1, 4.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
		m_heightmap->paintGround(m_position, 2, 4.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
		m_heightmap->paintGround(m_position, 3, 4.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
		m_heightmap->paintGround(m_position, 4, 4.0);
	}


}
