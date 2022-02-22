#include <vector>
#include <iostream>
#include "Projectile.h"

Mesh* Projectile::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    this->lengthX = length;
    this->lengthY = length;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3};


    square->InitFromData(vertices, indices);
    return square;
}

//check colission for square
bool Projectile::CheckCollision(float centerX, float centerY, Object* obj)
{
    // collision x-axis?
    bool collisionX = centerX + this->lengthX/2 >= obj->minX &&
        obj->minX + obj->lengthX >= centerX - this->lengthX/2;
    // collision y-axis?
    bool collisionY = centerY + this->lengthY/2 >= obj->minY &&
        obj->minY + obj->lengthY >= centerY - this->lengthY/2;

    // collision only if on both axes
    return collisionX && collisionY;
}

void Projectile::eliminate(int i) {
    this->times.erase(this->times.begin() + i);
    this->angles.erase(this->angles.begin() + i);
    this->playerX.erase(this->playerX.begin() + i);
    this->playerY.erase(this->playerY.begin() + i);
}