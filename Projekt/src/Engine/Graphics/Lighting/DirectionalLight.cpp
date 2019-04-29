#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() {
	m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	m_colour = glm::vec3(0.3f, 0.3f, 0.3f);
	m_ambientMultiplier = 0.5f;

	//Shader locations
	m_directionLoc = -1;
	m_colourLoc = -1;
	m_ambientMultiplierLoc = -1;
}

DirectionalLight::~DirectionalLight() {

}

void DirectionalLight::setColour(glm::vec3 newColour) {
	m_colour = newColour;
}

void DirectionalLight::setDirection(glm::vec3 newDirection) {
	m_direction = newDirection;
}

void DirectionalLight::setAmbientMultiplier(float newAmbientMultiplier) {
	m_ambientMultiplier = newAmbientMultiplier;
}

glm::vec3 DirectionalLight::getDirection() const{
	return m_direction;
}

float DirectionalLight::getAmbientMultiplier() const {
	return m_ambientMultiplier;
}

void DirectionalLight::setUniforms(GLuint shaderProgram) {
	if (m_directionLoc == -1)
		m_directionLoc = glGetUniformLocation(shaderProgram, "directionalLight.direction");
	if (m_colourLoc == -1)
		m_colourLoc = glGetUniformLocation(shaderProgram, "directionalLight.colour");
	if (m_ambientMultiplierLoc == -1)
		m_ambientMultiplierLoc = glGetUniformLocation(shaderProgram, "directionalLight.ambientMultiplier");


	glUniform3f(m_directionLoc, m_direction.x, m_direction.y, m_direction.z);
	glUniform3f(m_colourLoc, m_colour.x, m_colour.y, m_colour.z);
	glUniform1f(m_ambientMultiplierLoc, m_ambientMultiplier);
}