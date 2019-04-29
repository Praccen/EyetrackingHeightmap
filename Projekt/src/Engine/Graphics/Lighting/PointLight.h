#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <GL\glew.h>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <cstring>
#include <string>

class PointLight {
private:
	glm::vec3 m_position;
	glm::vec3 m_colour;

	int m_lightIndex;
	
	float m_constant;
	float m_linear;
	float m_quadratic;
	float m_radius;

	GLint m_positionLoc, m_colourLoc, m_constantLoc, m_linearLoc, m_quadraticLoc, m_radiusLoc;

public:
	PointLight();
	~PointLight();

	void setPosition(glm::vec3 newPosition);
	void setConstant(float newConstant);
	void setLinear(float newLinear);
	void setQuadratic(float newQuadratic);
	void setColour(glm::vec3 newColour);

	glm::vec3 getPosition();

	void setUniforms(GLuint shaderProgram, int lightIndex);
};

#endif