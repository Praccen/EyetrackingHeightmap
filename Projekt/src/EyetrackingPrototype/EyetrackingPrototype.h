#pragma once

#include "..\Engine\Application.h"
#include "Heightmap.h"

class EyetrackingPrototype : public Application {
private:
	Camera* m_camera;
	DirectionalLight* m_directionalLight;
	Heightmap* m_heightmap;

public:
	EyetrackingPrototype(sf::Window *originalWindow);
	~EyetrackingPrototype();

	void readKeys();

	void update(float dt);
};