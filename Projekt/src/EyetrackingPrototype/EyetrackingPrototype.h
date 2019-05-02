#pragma once

#include "..\Engine\Application.h"
#include "Heightmap.h"

class EyetrackingPrototype : public Application {
private:
	Camera* m_camera;
	float m_FOV;
	DirectionalLight* m_directionalLight;
	Heightmap* m_heightmap;
	sf::Window* m_window;

public:
	EyetrackingPrototype(sf::Window *originalWindow);
	~EyetrackingPrototype();

	glm::vec2 getMousePos();

	void update(float dt);
};