#pragma once
#include "Component.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_access.hpp>
#include <gtx/euler_angles.hpp>

class Transform : public Component {
public:
	glm::vec3 position = { 0, 0, 0 };
	glm::quat rotation = { 0, 0, 0, 1 };
	glm::vec3 scale = { 1, 1, 1 };

	void init() override {};

	void setRotationDegrees(float pitchDeg, float yawDeg, float rollDeg) {
		glm::vec3 rad = glm::radians(glm::vec3(pitchDeg, yawDeg, rollDeg));
		rotation = glm::quat(rad);
	}

	glm::vec3 getEulerDegrees() const {
		return glm::degrees(glm::eulerAngles(rotation));
	}
};