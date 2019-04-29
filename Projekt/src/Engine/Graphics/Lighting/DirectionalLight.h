#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <cstring>
#include <string>

class DirectionalLight {
private:
	glm::vec3 m_direction;
	glm::vec3 m_colour;
	float m_ambientMultiplier;

	//Shader locations
	GLint m_directionLoc, m_colourLoc, m_ambientMultiplierLoc;
public:
	DirectionalLight();
	~DirectionalLight();

	void setDirection(glm::vec3 newDirection);
	void setColour(glm::vec3 newColour);
	void setAmbientMultiplier(float newAmbientMultiplier);

	glm::vec3 getDirection() const;
	float getAmbientMultiplier() const;

	void setUniforms(GLuint shaderProgram);

};


#endif