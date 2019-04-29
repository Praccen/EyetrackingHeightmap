#ifndef CAMERA_H
#define CAMERA_H

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "..\..\Content\Object.h"

class Camera {
private:
	//Camera definitions
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraForward;
	glm::vec3 m_cameraUp;

	//Projection definitions
	float m_FOV;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
	glm::mat4 m_projection;

	//Flag
	bool m_hasTarget;

	//Target
	Object* m_target;
	glm::vec3 m_targetOffset;

public:
	Camera();
	~Camera();

	//Getters
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;
	glm::mat4 getProjection() const;
	glm::mat4 getView() const;
	Object* getTarget() const;
	glm::vec3 getTargetOffset() const;

	//Flag setters
	void useTarget(bool state);

	//Setters
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setTarget(Object* newTarget);
	void setTargetOffset(glm::vec3 offset);
	void setFOV(float FOV);

	void update();
};

#endif