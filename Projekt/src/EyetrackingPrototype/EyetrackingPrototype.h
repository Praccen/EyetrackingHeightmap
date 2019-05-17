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

	glm::vec2 m_eyePos;
	glm::vec2 m_lastMousePick;
	glm::vec2 m_lastValidPos;

	float m_timer;
	float m_precisionTimer;
	bool m_mouse;
	bool m_precisionAdjustment;
	bool testis;

public:
	EyetrackingPrototype(sf::Window *originalWindow, bool mouse);
	~EyetrackingPrototype();

	glm::vec2 getMousePos();

	void controlCamera(float dt);

	void setEyePos(glm::vec2 eyePos);

	void update(float dt);
};