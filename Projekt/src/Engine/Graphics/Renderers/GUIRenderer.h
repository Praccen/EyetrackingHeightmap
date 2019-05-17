#pragma once

#include <GL\glew.h>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


#include "..\Shader compilation\ShaderSet.h"


#include "..\..\Scene.h"

class GUIRenderer {
private:
	sf::Window *m_window;
	Scene *m_scene;

	ShaderSet* m_shaderSet;
public:
	GUIRenderer(sf::Window *originalWindow, Scene *scene);
	~GUIRenderer();

	void draw();
};