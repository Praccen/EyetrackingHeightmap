#include "EyetrackingPrototype.h"

EyetrackingPrototype::EyetrackingPrototype(sf::Window * originalWindow) : Application(originalWindow) {
	//Camera
	m_camera = Application::getCamera();
	m_camera->setFOV(65.0);
	m_camera->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	m_camera->setDirection(glm::vec3(1.0f, -0.5f, 1.0f));

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

void EyetrackingPrototype::update(float dt) {

}
