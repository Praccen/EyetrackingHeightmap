#pragma once

#include "..\Engine\Application.h"
#include "Heightmap.h"
#include "Menu/Menu.h"
#include "../Engine/Graphics/Renderers/DeferredRenderer.h"

class EyetrackingPrototype : public Application {
private:
	Camera* m_camera;
	DirectionalLight* m_directionalLight;
	Heightmap* m_heightmap;

	Menu menu;
	ShaderSet* menu_shader;

public:
	EyetrackingPrototype(sf::Window *originalWindow);
	~EyetrackingPrototype();

	void update(float dt);
};