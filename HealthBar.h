#pragma once

#include <string>

#include "Player.h"
#include "Object.h"
#include "Map.h"

class HealthBar:public Object{
public:
	Mesh* CreateHealthBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	Mesh* CreateRemainingHealthBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	float damageTaken = 0;
};