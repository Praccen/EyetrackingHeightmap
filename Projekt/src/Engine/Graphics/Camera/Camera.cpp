#include "Camera.h"

Camera::Camera() {
	//Camera definitions
	m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraForward = m_cameraFront;

	//Projection definitions
	m_FOV = 80.0f;
	m_aspectRatio = 16.0f / 9.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 300.0f;
	m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearPlane, m_farPlane);

	//Flags
	m_hasTarget = false;

	//Target
	m_target = nullptr;
	m_targetOffset = glm::vec3(1.0f, 1.0f, 1.0f);
}

Camera::~Camera() {

}

glm::vec3 Camera::getPosition() const {
	return m_cameraPos;
}

glm::vec3 Camera::getDirection() const {
	return glm::normalize(m_cameraFront);
}

glm::mat4 Camera::getProjection() const {
	return m_projection;
}

glm::mat4 Camera::getView() const {
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

Object* Camera::getTarget() const {
	return m_target;
}

glm::vec3 Camera::getTargetOffset() const {
	return m_targetOffset;
}

void Camera::useTarget(bool state) {
	if (state == true && m_target != nullptr) {
		m_hasTarget = true;
	}
	else {
		m_hasTarget = false;
	}
}

void Camera::setPosition(glm::vec3 position) {
	m_cameraPos = position;
}

void Camera::setDirection(glm::vec3 direction) {
	m_cameraFront = glm::normalize(direction);
}

void Camera::setTarget(Object* newTarget) {
	m_target = newTarget;
}

void Camera::setTargetOffset(glm::vec3 offset) {
	m_targetOffset = offset;
}

void Camera::setFOV(float FOV) {
	m_FOV = FOV;
	m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::update() {
	if (m_hasTarget) {
		m_cameraPos = m_target->getPosition() + m_targetOffset;
		m_cameraFront = m_target->getPosition() - m_cameraPos;
	}
}