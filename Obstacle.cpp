#include "Obstacle.h"
#include <vector>
#include <iostream>

Mesh* Obstacle::CreateObstacle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float lengthX,
    float lengthY,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    this->minX = corner[0];
    this->minY = corner[1];
    this->lengthX = lengthX;
    this->lengthY = lengthY;
    
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(lengthX, 0, 0), color),
        VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), color),
        VertexFormat(corner + glm::vec3(0, lengthY, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3};

    square->InitFromData(vertices, indices);
    return square;
}
