#include "Enemy.h"
#include <vector>
#include <iostream>

Mesh* Enemy::CreateEnemy(
    const std::string& name,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = glm::vec3(0,0,0);

    if (lengthX == 0.0 && lengthY == 0.0) {
        this->lengthX = length;
        this->lengthY = length;
        this->length_eye = length / 3;
    }
    
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
//1 deciamal
float Enemy::randomValue(int start, int end) {
    start = start * 10;
    end = end * 10;
    int result = start + (std::rand() * 10 % (end - start + 1));
    return (float)result / 10;
}
//ads enemies based on the level for example for level 6 ads 6 enemies.
void Enemy::addEnemy(int playerX, int playerY, int diameter) {
    if (nr == max_nr) {
        return;
    }
    //only when all the enemies are dead go to next level
    if (centerX.size() == 0) {
        nr += 1;
        for (int i = 0; i < nr; i++) {
            float x, y;
            x = randomValue(map->minX, map->minX + map->lengthX);
            y = randomValue(map->minY, map->minY + map->lengthY);

            //can't spawn enemies too close to the player
            while (x > playerX - 2*diameter && x < playerX + 2 * diameter) {
                x = randomValue(map->minX, map->minX + map->lengthX);
            }
            while (y > playerY -  2*diameter && y < playerY + 2 * diameter) {
                y = randomValue(map->minY, map->minY + map->lengthY);
            }

            this->centerX.push_back(x);
            this->centerY.push_back(y);
            this->speed.push_back(randomValue(50, 130));
        }
    }
}

void Enemy::eliminate(int enemy_index) {
    this->centerX.erase(this->centerX.begin() + enemy_index);
    this->centerY.erase(this->centerY.begin() + enemy_index);
    this->speed.erase(this->speed.begin() + enemy_index);
}