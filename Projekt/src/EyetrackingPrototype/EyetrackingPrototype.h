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

	GUIElement* m_testGUIElement;

	glm::vec2 m_eyePos;
	glm::vec2 m_lastMousePick;

	float m_timer;
	bool m_mouse;

public:
	EyetrackingPrototype(sf::Window *originalWindow, bool mouse);
	~EyetrackingPrototype();

	glm::vec2 getMousePos();

	void controlCamera(float dt);

	void setEyePos(glm::vec2 eyePos);

	void menuInteraction();

	void update(float dt);
};