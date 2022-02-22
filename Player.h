#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "Object.h"
#include "Enemy.h"

class Player{
public:
	//create the player by 3 circles
	Mesh* CreateCircle(
		const std::string& name,
		float radius,
		glm::vec3 color);
	//how many triangles does on cirle has
	const int steps = 100;
	//the angle of a triangle
	const float angle = 3.1415926 * 2.f / steps;
	//the position on XoY of the center of the circle
	float x, y;
	//how much to rotate to catch the mouse's cursor
	float rotateAngle = 0;
	float radius;
	//the 3 circles reprezenting the player
	Mesh* body;
	Mesh* leftEye;
	Mesh* rightEye;
	float left_eye_x;
	float left_eye_y;
	float right_eye_x;
	float right_eye_y;
	//check collision with a an object
	bool CheckCollision(Object* object);
	//check collision with an enemy
	bool CheckCollisionEnemy(int index, Enemy* enemy);
};