#include "PointLight.h"

PointLight::PointLight() {
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_colour = glm::vec3(1.0f, 1.0f, 1.0f);

	m_lightIndex = -1;

	m_constant = 1.0f;
	m_linear = 0.07f;
	m_quadratic = 0.017f;
	m_radius = (-m_linear + std::sqrtf(m_linear * m_linear - 4 * m_quadratic * (m_constant - (256.0f / 5.0f)))) / (2.0f * m_quadratic);

	m_positionLoc = -1;
	m_colourLoc = -1;
	m_constantLoc = -1;
	m_linearLoc = -1;
	m_quadraticLoc = -1;
	m_radiusLoc = -1;
}

PointLight::~PointLight() {
	
}

void PointLight::setPosition(glm::vec3 newPosition) {
	m_position = newPosition;
}

void PointLight::setConstant(float newConstant) {
	m_constant = newConstant;
}

void PointLight::setLinear(float newLinear) {
	m_linear = newLinear;
}

void PointLight::setQuadratic(float newQuadratic) {
	m_quadratic = newQuadratic;
}

void PointLight::setColour(glm::vec3 newColour) {
	m_colour = newColour;
}

glm::vec3 PointLight::getPosition() {
	return m_position;
}

void PointLight::setUniforms(GLuint shaderProgram, int lightIndex) {
	if (lightIndex != m_lightIndex) {
		std::string s = "pointLights[" + std::to_string(lightIndex) + "].position";
		m_positionLoc = glGetUniformLocation(shaderProgram, s.c_str());
		s = "pointLights[" + std::to_string(lightIndex) + "].colour";
		m_colourLoc = glGetUniformLocation(shaderProgram, s.c_str());
		s = "pointLights[" + std::to_string(lightIndex) + "].constant";
		m_constantLoc = glGetUniformLocation(shaderProgram, s.c_str());
		s = "pointLights[" + std::to_string(lightIndex) + "].linear";
		m_linearLoc = glGetUniformLocation(shaderProgram, s.c_str());
		s = "pointLights[" + std::to_string(lightIndex) + "].quadratic";
		m_quadraticLoc = glGetUniformLocation(shaderProgram, s.c_str());
		s = "pointLights[" + std::to_string(lightIndex) + "].radius";
		m_radiusLoc = glGetUniformLocation(shaderProgram, s.c_str());
		m_lightIndex = lightIndex;
	}
	
	glUniform3f(m_positionLoc, m_position.x, m_position.y, m_position.z);
	glUniform3f(m_colourLoc, m_colour.x, m_colour.y, m_colour.z);
	glUniform1f(m_constantLoc, m_constant);
	glUniform1f(m_linearLoc, m_linear);
	glUniform1f(m_quadraticLoc, m_quadratic);
	glUniform1f(m_radiusLoc, m_radius);
}