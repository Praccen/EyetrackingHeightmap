#pragma once

#include <GL\glew.h>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <fstream>

#include "..\Shader compilation\ShaderSet.h"

#include "..\..\Scene.h"

class DeferredRenderer {
private:
	sf::Window *m_window;
	Scene *m_scene;

	//Shader sets needed
	ShaderSet* m_menuShaders;
	ShaderSet* m_gBufferShaders;
	ShaderSet* m_shadowShaders;
	ShaderSet* m_lightingShaders;

	//gBuffer
	GLuint m_gBuffer; 
	GLuint m_gPosition, m_gNormal, m_gColourSpec;
	GLuint m_rboDepth;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;

	//shadow mapping
	GLuint m_shadowBuffer;
	GLuint m_depthMap;
	int m_shadowWidth, m_shadowHeight;
	glm::mat4 m_lightView, m_lightProjection, m_lightSpace, m_biasMatrix;
	glm::vec3 m_lightOffset;

	//Shader locations
	GLint m_sLightSpaceMatrixLoc, m_lLightSpaceMatrixLoc;

	struct Vertex {
		float x, y, z; //Position
		float u, v; //TexCoords
	};

	//Create buffers
	void createGBuffer();
	void createShadowBuffer();

	//Set up rendering passes
	void setGeometryPass();
	void setShadowPass();
	void setLightPass();

	//Render all objects
	void renderScene(GLuint shaderProgram);

	//Render onto quad
	void renderQuad();
public:
	DeferredRenderer(sf::Window *originalWindow,  Scene *scene);
	~DeferredRenderer();
	
	ShaderSet* getMenuPointer() const;

	void draw();
};