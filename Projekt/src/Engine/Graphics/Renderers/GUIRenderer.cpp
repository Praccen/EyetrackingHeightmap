#include "GUIRenderer.h"

GUIRenderer::GUIRenderer(sf::Window *originalWindow, Scene *scene) {
	m_scene = scene;
	m_window = originalWindow;

	m_shaderSet = new ShaderSet("GUI_Vertex.glsl", "GUI_Frag.glsl");

	m_shaderSet->use();
}

GUIRenderer::~GUIRenderer() {
	delete m_shaderSet;
}

void GUIRenderer::draw() {
	m_scene->drawGUI(m_shaderSet->getShaderProgram());
}