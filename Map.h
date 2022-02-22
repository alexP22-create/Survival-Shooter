#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "Object.h"
class Map:public Object{
public:
	//create the player by 3 circles
	Mesh* CreateMap(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	//map's edges are mapps with the height or width 0
	std::vector<Map*> map_edges = std::vector<Map*>(4);
};