#include "Scene.h"

Scene::Scene() {
	m_nrOfPointLights = 0;

	//Geometry shader locations
	m_projLoc = -1;
	m_viewLoc = -1;
	m_geometryCameraPosLoc = -1;

	//Lighting shader locations
	m_lightCameraPosLoc = -1;
	m_nrOfPointLightsLoc = -1;
}

Scene::~Scene() {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		delete m_objects[i];
	}
}

int Scene::getNrOfPointLights() {
	return m_nrOfPointLights;
}

Object* Scene::addObject() {
	Object* object = new Object(&m_modelHandler);
	m_objects.push_back(object);
	return object;
}

Camera* Scene::getCamera() {
	return &m_camera;
}

DirectionalLight* Scene::getDirectionalLight() {
	return &m_directionalLight;
}

void Scene::setGeometryUniforms(GLuint shaderProgram) {
	if (m_projLoc == -1)
		m_projLoc = glGetUniformLocation(shaderProgram, "projection");
	if (m_viewLoc == -1)
		m_viewLoc = glGetUniformLocation(shaderProgram, "view");
	if (m_geometryCameraPosLoc == -1)
		m_geometryCameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");

	glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getProjection()));
	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getView()));
	glUniform3f(m_geometryCameraPosLoc, m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
}

void Scene::setShadowUniforms(GLuint shaderProgram) {

}

void Scene::setLightingUniforms(GLuint shaderProgram) {
	if (m_lightCameraPosLoc == -1)
		m_lightCameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
	if (m_nrOfPointLightsLoc == -1)
		m_nrOfPointLightsLoc = glGetUniformLocation(shaderProgram, "nrOfPointLights");

	glUniform3f(m_lightCameraPosLoc, m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
	m_directionalLight.setUniforms(shaderProgram);

	m_nrOfPointLights = 0;
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		if (m_objects[i]->isLit()) {
			m_objects[i]->getPointLight()->setUniforms(shaderProgram, m_nrOfPointLights);
			m_nrOfPointLights++;
		}
	}
	glUniform1i(m_nrOfPointLightsLoc, m_nrOfPointLights);
}

void Scene::update(float dt) {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		m_objects[i]->update(dt);
	}
	m_camera.update();
}

void Scene::draw(GLuint shaderProgram) {
	for (unsigned int i = 0; i < m_objects.size(); i++) {
		m_objects[i]->draw(shaderProgram);
	}
}