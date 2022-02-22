#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "Map.h"
#include <vector>;

class Enemy:public Object{
public:
	//square
	Mesh* CreateEnemy(
		const std::string& name,
		float length,
		glm::vec3 color);

	float left_eye_x;
	float right_eye_x;
	float left_eye_y;
	float right_eye_y;

	float length_eye;

	std::vector <float> speed;

	Map* map;

	//the coordonates of the center of the current position of every alive enemy
	std::vector <float> centerX;
	std::vector <float> centerY;

	//adds enemies based on the time and the location of the player
	void addEnemy(int playerX, int playerY, int diameter);
	
	float randomValue(int start, int end);

	const int max_nr = 7;
	const int nrEnemies = max_nr * (max_nr + 1) / 2; 
	int nr = 0;
	const float damage = 30;
	void eliminate(int index);
};