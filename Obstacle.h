#pragma once

#include <string>

#include "Player.h"
#include "Object.h"
#include "Map.h"

class Obstacle:public Object{
public:
	//square
	Mesh* CreateObstacle(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	Map* edgeUp;
	Map* edgeDown;
	Map* edgeLeft;
	Map* edgeRight;

};