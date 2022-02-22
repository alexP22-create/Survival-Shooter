#include "Player.h"
#include <vector>
#include <iostream>

Mesh* Player::CreateCircle(const std::string& name, float radius, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    //save the biggest radius
    if (this->radius < radius) {
        this->radius = radius;
    }
    int index = 0;

    float centerX = 0;
    float centerY = 0;

    float prevX = centerX;
    float prevY = centerY - radius;

    //build the big circle
    for (int i = 0; i <= steps; i++) {
        float newX = sin(angle * i) * radius;
        float newY = (-1) * cos(angle * i) * radius;

        //first add the center and the starting point of the circle
        vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(prevX, prevY, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(newX, newY, 0), color));

        indices.push_back(index++);
        indices.push_back(index++);
        indices.push_back(index++);

        prevX = newX;
        prevY = newY;
    }

    //create the mesh;
    Mesh* my_mesh = new Mesh(name);
    my_mesh->InitFromData(vertices, indices);
    return my_mesh;
}

bool Player::CheckCollision(Object* obj) 
{
    // get center point circle first 
    glm::vec2 center(this->x, this->y);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(obj->lengthX / 2.0f, obj->lengthY / 2.0f);
    glm::vec2 aabb_center(
        obj->minX + aabb_half_extents.x,
        obj->minY + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < this->radius;
}

bool Player::CheckCollisionEnemy(int index, Enemy* enemy)
{
    // get center point circle first 
    glm::vec2 center(x, y);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(enemy->lengthX / 2.0f, enemy->lengthY / 2.0f);
    glm::vec2 aabb_center(
        enemy->centerX[index] + aabb_half_extents.x,
        enemy->centerY[index] + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < this->radius;
}
