#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "Object.h"

class ScoreBar:public Object{
public:
	Mesh* CreateScoreBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	Mesh* CreateGainedScoreBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	//total gaines score
	float gainedScore;
	//the scor gained per killing an enemy
	float score_per_enemy;
};