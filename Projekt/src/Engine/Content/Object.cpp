#include "Object.h"

//----Member functions----
void Object::updateModelMatrix() {
	m_modelMatrix = glm::mat4();
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation, m_rotationAxis);
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	m_modelMatrixHasChanged = true;
}
//------------------------

//----Public functions----
Object::Object(ModelHandler* modelHandler) {
	m_modelHandler = modelHandler;

	m_model = nullptr;

	//Flags
	m_modelIsActive = false;
	m_recalculateBoundingBox = false;
	m_isLit = false;
	m_planeIsActive = false;
	m_meshBuilderIsActive = false;

	//Properties
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_modelMatrix = glm::mat4();
}

Object::~Object() {

}


//Flag setters
void Object::setModelVisable(bool state) {
	if (state == true && m_model != nullptr) { //Make sure a model has been loaded before the m_hasModel flag can be set to true
		m_modelIsActive = true;
		m_model->setModelMatrix(m_modelMatrix);
	}
	else {
		m_modelIsActive = false;
	}
}

void Object::setBoundingBoxActive(bool state) {
	m_recalculateBoundingBox = state;
	m_boundingBox.setModelMatrix(m_modelMatrix);
}

void Object::setPointLightActive(bool state) {
	m_isLit = state;
	m_pointLight.setPosition(m_position);
}

void Object::setPlaneActive(bool state) {
	m_planeIsActive = state;
	m_plane.setModelMatrix(m_modelMatrix);
}

void Object::setMeshBuilderActive(bool state) {
	m_meshBuilderIsActive = state;
	m_meshBuilder.setModelMatrix(m_modelMatrix);
}


//Data setters
void Object::setModel(std::string modelPath) {
	m_model = m_modelHandler->getModel(modelPath);
}

void Object::setBoundingBox(glm::vec3 minCorner, glm::vec3 maxCorner) {
	m_boundingBox.createBoundingBox(minCorner, maxCorner);
}

void Object::useModelBoundingBox() {
	if (m_model != nullptr) {
		m_boundingBox.createBoundingBox(m_model->getSize()[0], m_model->getSize()[1]);
	}
}


//Property setters
void Object::setPosition(glm::vec3 position) {
	m_position = position;
	updateModelMatrix();
}

void Object::setScale(glm::vec3 scale) {
	m_scale = scale;
	updateModelMatrix();
}

void Object::setRotation(float rotationAmount, glm::vec3 rotationAxis) {
	m_rotation = rotationAmount;
	m_rotationAxis = rotationAxis;
	updateModelMatrix();
}

void Object::setModelMatrix(glm::mat4 modelMatrix) {
	m_modelMatrix = modelMatrix;
	m_position = glm::vec3(m_modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_modelMatrixHasChanged = true;
}


//Flag getters
bool Object::isLit() const {
	return m_isLit;
}


//Property getters
glm::vec3 Object::getPosition() const {
	return m_position;
}

float Object::getRotation() const {
	return m_rotation;
}

glm::vec3 Object::getRotationAxis() const {
	return m_rotationAxis;
}


//Component getters
PointLight* Object::getPointLight() {
	return &m_pointLight;
}

Plane* Object::getPlane() {
	return &m_plane;
}

MeshBuilder* Object::getMeshBuilder() {
	return &m_meshBuilder;
}

BoundingBox* Object::getBoundingBox() {
	return &m_boundingBox;
}


//Updates
void Object::update(float dt) {
	if (m_modelMatrixHasChanged) {
		if (m_modelIsActive) {
			m_model->setModelMatrix(m_modelMatrix);
		}

		if (m_planeIsActive) {
			m_plane.setModelMatrix(m_modelMatrix);
		}

		if (m_meshBuilderIsActive) {
			m_meshBuilder.setModelMatrix(m_modelMatrix);
		}

		if (m_recalculateBoundingBox) {
			m_boundingBox.setModelMatrix(m_modelMatrix);
			m_boundingBox.recalculateBoundingBox();
		}

		if (m_isLit) {
			m_pointLight.setPosition(m_position);
		}

		m_modelMatrixHasChanged = false;
	}
}


//Draw
void Object::draw(GLuint shaderProgram) {
	if (m_modelIsActive) {
		m_model->draw(shaderProgram);
	}

	if (m_planeIsActive) {
		m_plane.draw(shaderProgram);
	}

	if (m_meshBuilderIsActive) {
		m_meshBuilder.draw(shaderProgram);
	}
}
//------------------------