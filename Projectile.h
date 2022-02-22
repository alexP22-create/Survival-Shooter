#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include "Object.h"
#include <vector>

class Projectile:public Object{
public:
	Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);

	//check colision with an obj
	bool CheckCollision(float x, float y, Object* object);
	//elimina projectile
	void eliminate(int index);
	//timp-ul de existenta al unei proiectile
	const float timeMax = 2.3;
	//the times of life for each projectile
	std::vector <float> times;
	//the angle for each projectile
	std::vector <float> angles;
	//the coordonates of the fire position
	std::vector <float> playerX;
	std::vector <float> playerY;
	//the projections of the 
	float x, y;
	//miliseconds
	float fireRate = 500;

};