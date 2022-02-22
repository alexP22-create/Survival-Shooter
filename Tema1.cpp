#include "Tema1.h"

#include <vector>
#include <iostream>

#include "transform2D.h"
#include "Player.h"
#include "Map.h"
#include "Obstacle.h"
#include "Projectile.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

//check colission betwwn enemy and an object
bool Tema1::CheckCollision(int enemy_index, Object* obj)
{
    // collision x-axis?
    bool collisionX = enemy->centerX[enemy_index] + enemy->lengthX / 2 >= obj->minX &&
        obj->minX + obj->lengthX >= enemy->centerX[enemy_index] - enemy->lengthX / 2;
    // collision y-axis?
    bool collisionY = enemy->centerY[enemy_index] + enemy->lengthY / 2 >= obj->minY &&
        obj->minY + obj->lengthY >= enemy->centerY[enemy_index] - enemy->lengthY / 2;

    // collision only if on both axes
    return collisionX && collisionY;
}

//check colission between enemy and projectile
bool Tema1::CheckCollisionProjectile(int enemy_index, int p)
{
    float minX = projectile->playerX[p] - projectile->lengthX / 2;
    float minY = projectile->playerY[p] - projectile->lengthY / 2;
    // collision x-axis?
    bool collisionX = enemy->centerX[enemy_index] + enemy->lengthX / 2 >= minX &&
        minX + projectile->lengthX >= enemy->centerX[enemy_index] - enemy->lengthX / 2;
    // collision y-axis?
    bool collisionY = enemy->centerY[enemy_index] + enemy->lengthY >= minY &&
        minY + projectile->lengthY >= enemy->centerY[enemy_index] - enemy->lengthY / 2;

    // collision only if on both axes
    return collisionX && collisionY;
}

//the map edges are mapps which look like a line, used for checking the collision with the exterior of the map
void Tema1::initializeMapEdges() {
    map->map_edges[0] = new Map();
    Mesh* mapMesh1 = map->map_edges[0]->CreateMap("edgeUp", glm::vec3(map->minX, map->minY + map->lengthY, 0), map->lengthX, 0.1, glm::vec3(0, 0, 0));
    AddMeshToList(mapMesh1);

    map->map_edges[1] = new Map();
    Mesh* mapMesh2 = map->map_edges[1]->CreateMap("edgeLeft", glm::vec3(map->minX, map->minY, 0), 0.1, map->lengthY, glm::vec3(0, 0, 0));
    AddMeshToList(mapMesh2);

    map->map_edges[2] = new Map();
    Mesh* mapMesh3 = map->map_edges[2]->CreateMap("edgeDown", glm::vec3(map->minX, map->minY, 0), map->lengthX, 0.1, glm::vec3(0, 0, 0));
    AddMeshToList(mapMesh3);

    map->map_edges[3] = new Map();
    Mesh* mapMesh4 = map->map_edges[3]->CreateMap("edgeRight", glm::vec3(map->minX + map->lengthX, map->minY, 0), 0.1, map->lengthY, glm::vec3(0, 0, 0));
    AddMeshToList(mapMesh4);
}
//for every object use the object's edges for finding the way in which the player can move at collision with the obstacles
void Tema1::initializeObstacleEdges() {
    for (int i = 0; i < obstacles.size(); i++) {
        obstacles[i]->edgeUp = new Map();
        Mesh* up = obstacles[i]->edgeUp->CreateMap("edgeUp"+i, glm::vec3(obstacles[i]->minX, obstacles[i]->minY + obstacles[i]->lengthY, 0), obstacles[i]->lengthX, 0, glm::vec3(0, 1, 0));
        AddMeshToList(up);

        obstacles[i]->edgeLeft = new Map();
        Mesh* left = obstacles[i]->edgeLeft->CreateMap("edgeLeft"+i, glm::vec3(obstacles[i]->minX, obstacles[i]->minY, 0), 0, obstacles[i]->lengthY, glm::vec3(0, 1, 0));
        AddMeshToList(left);

        obstacles[i]->edgeDown = new Map();
        Mesh* down = obstacles[i]->edgeDown->CreateMap("edgeDown"+i, glm::vec3(obstacles[i]->minX, obstacles[i]->minY, 0), obstacles[i]->lengthX, 0, glm::vec3(0, 1, 0));
        AddMeshToList(down);

        obstacles[i]->edgeRight = new Map();
        Mesh* mapMesh4 = obstacles[i]->edgeRight->CreateMap("edgeRight"+i, glm::vec3(obstacles[i]->minX + obstacles[i]->lengthX, obstacles[i]->minY, 0), 0, obstacles[i]->lengthY, glm::vec3(0, 1, 0));
        AddMeshToList(mapMesh4);
    }
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);

    map = new Map();
    Mesh* mapMesh = map->CreateMap("map", glm::vec3(0, 0, 0), 700, 900, glm::vec3(0.6, 0.6, 0.6));
    AddMeshToList(mapMesh);

    this->initializeMapEdges();

    logicSpace.width = 400;   // logic width
    logicSpace.height = 400;  // logic height
    //calculate the coord of the logicSpace which is in the center of the map
    logicSpace.x = map->lengthX / 2 + map->minX - logicSpace.width / 2;// logic x
    logicSpace.y = map->lengthY / 2 + map->minY - logicSpace.height / 2;       // logic y

    player = new Player();
    player->x = logicSpace.x + logicSpace.width / 2;
    player->y = logicSpace.y + logicSpace.height / 2;
    Mesh* playerBodyMesh = player->CreateCircle("player_body", 20, glm::vec3(1, 1, 0));
    AddMeshToList(playerBodyMesh);
    //set the initial coord othe eyes
    float CX = 0.5 * player->radius;
    float CY = sqrt(3) / 2 * player->radius;
    player->left_eye_x = -CX;
    player->left_eye_y = -CY;
    player->right_eye_x = +CX;
    player->right_eye_y = -CY;
    Mesh* playerLeftEyeMesh = player->CreateCircle("player_left_eye", player->radius / 3, glm::vec3(1, 0, 0));
    AddMeshToList(playerLeftEyeMesh);
    Mesh* playerRightEyeMesh = player->CreateCircle("player_right_eye", player->radius / 3, glm::vec3(1, 0, 0));
    AddMeshToList(playerRightEyeMesh);

    //create 4 obstacles
    //i am using a different object for every obstacle unlike i do to the enemies or projectiles because every
    //obstacle is different, doesn't dissapear and i need the edges of all of them meanwhile every enemy and
    //projectile is and behaves the same way.
    obstacles[0] = new Obstacle();
    Mesh* obstacle = obstacles[0]->CreateObstacle("obstacle1", glm::vec3(200, 550, 0), 50, 50, glm::vec3(0, 1, 0));
    AddMeshToList(obstacle);
    obstacles[1] = new Obstacle();
    obstacle = obstacles[1]->CreateObstacle("obstacle2", glm::vec3(450, 700, 0), 150, 100, glm::vec3(0, 1, 0));
    AddMeshToList(obstacle);
    obstacles[2] = new Obstacle();
    obstacle = obstacles[2]->CreateObstacle("obstacle3", glm::vec3(100, 100, 0), 50, 150, glm::vec3(0, 1, 0));
    AddMeshToList(obstacle);
    obstacles[3] = new Obstacle();
    obstacle = obstacles[3]->CreateObstacle("obstacle4", glm::vec3(150, 100, 0), 200, 50, glm::vec3(0, 1, 0));
    AddMeshToList(obstacle);
    obstacles[4] = new Obstacle();
    obstacle = obstacles[4]->CreateObstacle("obstacle5", glm::vec3(400, 300, 0), 300, 70, glm::vec3(0, 1, 0));
    AddMeshToList(obstacle);

    initializeObstacleEdges();

    //create projectile mesh
    projectile = new Projectile();
    Mesh* projectileMesh = projectile->CreateSquare("projectile", glm::vec3(0, 0, 0), player->radius, glm::vec3(0, 1, 1));
    AddMeshToList(projectileMesh);

    //create enemy
    enemy = new Enemy();
    Mesh* enemyCenter = enemy->CreateEnemy("enemy_body", 40, glm::vec3(1, 0, 1));
    AddMeshToList(enemyCenter);
    enemy->map = map;

    enemy->right_eye_x = enemy->lengthX - enemy->length_eye;
    enemy->right_eye_y = -enemy->length_eye;

    enemy->left_eye_x = 0;
    enemy->left_eye_y = -enemy->length_eye;

    Mesh* enemyLeftEye = enemy->CreateEnemy("enemy_left_eye", enemy->length_eye, glm::vec3(0, 0, 1));
    AddMeshToList(enemyLeftEye);
    Mesh* enemyRightEye = enemy->CreateEnemy("enemy_right_eye", enemy->length_eye, glm::vec3(0, 0, 1));
    AddMeshToList(enemyRightEye);

    //init healthbar
    HB = new HealthBar();
    Mesh* healthContour = HB->CreateHealthBar("health_contour", glm::vec3(0, 0, 0), 150, 15, glm::vec3(1,0,0));
    AddMeshToList(healthContour);
    Mesh* health = HB->CreateRemainingHealthBar("health", glm::vec3(0, 0, 0), HB->lengthX, HB->lengthY, glm::vec3(1, 0, 0));
    AddMeshToList(health);

    //init score bar
    SB = new ScoreBar();
    SB->score_per_enemy = HB->lengthX / enemy->nrEnemies;
    Mesh* scoreContour = SB->CreateScoreBar("score_contour", glm::vec3(0, 0, 0), HB->lengthX, HB->lengthY, glm::vec3(0, 0, 1));
    AddMeshToList(scoreContour);
    Mesh* score = SB->CreateGainedScoreBar("score", glm::vec3(0, 0, 0), 0, HB->lengthY, glm::vec3(0, 0, 1));
    AddMeshToList(score);

    begin_time = clock();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}
//checks the collisions of an enemy with the player, the projectiles and the map edges
int Tema1::checkMapCollisionsForEnemies(int enemy_index, float angle) {
    if (player->CheckCollisionEnemy(enemy_index, this->enemy)) {
        HB->damageTaken += 30.0f;
        if (HB->damageTaken > HB->lengthX) {
            HB->damageTaken = HB->lengthX;
        }

        meshes.erase("health");
        Mesh* health = HB->CreateRemainingHealthBar("health", glm::vec3(0, 0, 0), HB->lengthX - HB->damageTaken, HB->lengthY, glm::vec3(1, 0, 0));
        AddMeshToList(health);

        enemy->eliminate(enemy_index);
        return 1;
    }
    //checks the collisions with every projectile
    for (int i = 0; i < projectile->angles.size(); i++) {

        if (CheckCollisionProjectile(enemy_index, i)) {

            SB->gainedScore += SB->score_per_enemy;
            if (SB->gainedScore > SB->lengthX) {
                SB->gainedScore = SB->lengthX;
            }

            meshes.erase("score");
            Mesh* score = HB->CreateRemainingHealthBar("score", glm::vec3(0, 0, 0), SB->gainedScore, SB->lengthY, glm::vec3(0, 0, 1));
            AddMeshToList(score);

            projectile->eliminate(i);
            enemy->eliminate(enemy_index);
            return 1;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (CheckCollision(enemy_index, map->map_edges[0])) {
            return 2;
        }
        if (CheckCollision(enemy_index, map->map_edges[1])) {
            return 3;
        }
        if (CheckCollision(enemy_index, map->map_edges[2])) {
            return 4;
        }
        if (CheckCollision(enemy_index, map->map_edges[3])) {
            return 5;
        }
    }
    return 0;
}
//draws the enemy to his new position
void Tema1::moveEnemy(int i, float angle) {
    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(enemy->centerX[i], enemy->centerY[i]) *
        transform2D::Rotate(angle) *
        transform2D::Translate(enemy->left_eye_x, enemy->left_eye_y) *
        transform2D::Translate(-enemy->lengthX / 2, -enemy->lengthX / 2) *
        modelMatrix;
    RenderMesh2D(meshes["enemy_left_eye"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(enemy->centerX[i], enemy->centerY[i]) *
        transform2D::Rotate(angle) *
        transform2D::Translate(enemy->right_eye_x, enemy->right_eye_y) *
        transform2D::Translate(-enemy->lengthX / 2, -enemy->lengthX / 2) *
        modelMatrix;
    RenderMesh2D(meshes["enemy_right_eye"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(enemy->centerX[i], enemy->centerY[i]) *
        transform2D::Rotate(angle) *
        transform2D::Translate(-enemy->lengthX / 2, -enemy->lengthX / 2) *
        modelMatrix;
    RenderMesh2D(meshes["enemy_body"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DisplayEnemy(glm::mat3 visMatrix, float deltaTimeSeconds) {

    for (int i = 0; i < enemy->centerX.size(); i++) {
        //the enemies are orientated towards the player so we have to calculate the angle
        float distX = player->x - enemy->centerX[i];
        float distY = player->y - enemy->centerY[i];
        double angle = 3.14 - atan2(distX, distY);

        //the speed
        float enemy_distance = deltaTimeSeconds * enemy->speed[i];

        //find the difference between the initial coordonates and the ones of the next position
        //-pi/2 because initially an enemy is oriented downwards
        float ox = enemy_distance * cos(angle - 3.14/2);
        float oy = enemy_distance * sin(angle - 3.14/2);

        enemy->centerX[i] += ox;
        enemy->centerY[i] += oy;

        int collisionType = checkMapCollisionsForEnemies(i, angle);
        if (collisionType == 0) {
            moveEnemy(i, angle);
        }
        if (collisionType == 2 && oy < 0) {
            moveEnemy(i, angle);
        }
        if (collisionType == 3 && ox > 0) {
            moveEnemy(i, angle);
        }
        if (collisionType == 4 && oy > 0) {
            moveEnemy(i, angle);
        }
        if (collisionType == 5 && ox < 0) {
            moveEnemy(i, angle);
        }
        
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    //display the projectiles
    for (int i = 0; i < projectile->angles.size(); i++) {
        //calculate coordinates of the points of the final trajectory of a projectile
        float distance = 2 * deltaTimeSeconds * 100;
        // - pi/2 because my player is oriented downwards
        float onOx = distance * cos(projectile->angles[i] - 3.14/2);
        float onOy = distance * sin(projectile->angles[i] - 3.14/2);
        //new position of a projectile
        projectile->playerX[i] += onOx;
        projectile->playerY[i] += onOy;
        modelMatrix = glm::mat3(1);
        modelMatrix = visMatrix *
            transform2D::Translate(projectile->playerX[i], projectile->playerY[i]) *
            transform2D::Rotate(projectile->angles[i]) *
            transform2D::Translate(-projectile->lengthX/2, -projectile->lengthY/2) *
            modelMatrix;
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        float valX = projectile->playerX[i];
        float valY = projectile->playerY[i];

        projectile->times[i] += 2*deltaTimeSeconds;
        //get the current position of every projectile to search for collisions
        bool collision = false;
        //check collisions with every map edge
        for (int j = 0; j < map->map_edges.size(); j++) {
            if (projectile->CheckCollision(valX, valY,map->map_edges[j])) {
                collision = true;
            }
        }
        //check collisions with every object
        for (int j = 0; j < obstacles.size(); j++) {
            if (projectile->CheckCollision(valX, valY, obstacles[j])) {
                collision = true;
            }
        }

        if (projectile->times[i] > projectile->timeMax || collision) {
            //remove from the array
            projectile->eliminate(i);
        }
    }
    
    //display the player
    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(player->x, player->y) *
        transform2D::Rotate(player->rotateAngle) *
        transform2D::Translate(player->left_eye_x, player->left_eye_y) *
        modelMatrix;
    RenderMesh2D(meshes["player_left_eye"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(player->x, player->y) *
        transform2D::Rotate(player->rotateAngle) *
        transform2D::Translate(player->right_eye_x, player->right_eye_y) *
        modelMatrix;
    RenderMesh2D(meshes["player_right_eye"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix = visMatrix *
        transform2D::Translate(player->x, player->y) *
        transform2D::Rotate(player->rotateAngle) *
        modelMatrix;
    RenderMesh2D(meshes["player_body"], shaders["VertexColor"], modelMatrix);

    //display enemies
    DisplayEnemy(visMatrix, deltaTimeSeconds);
    //display de objects
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle5"], shaders["VertexColor"], modelMatrix);

    
    //display the map and the map edges
    modelMatrix = visMatrix *
        glm::mat3(1);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix *
        glm::mat3(1);
    RenderMesh2D(meshes["edgeUp"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix *
        glm::mat3(1);
    RenderMesh2D(meshes["edgeLeft"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix *
        glm::mat3(1);
    RenderMesh2D(meshes["edgeDown"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix *
        glm::mat3(1);
    RenderMesh2D(meshes["edgeRight"], shaders["VertexColor"], modelMatrix);
}
//checks if the game ended
void Tema1::CheckEndGame() {
    if (enemy->centerX.size() == 0 && HB->damageTaken < HB->lengthX && gameWon == false) {
        std::cout << "Congratulations! You won with the score ";
        std::cout << SB->gainedScore;
        std::cout << " of ";
        std::cout << SB->lengthX;
        std::cout << '\n';
        std::cout << "Press C on the keyboard to EXIT\n";
        gameWon = true;
    }
    if (HB->damageTaken >= HB->lengthX) {
        std::cout << ":(( You lost!\n";
        std::cout << "The game stopped. Please try again :]]\n";
        exit(0);
    }
}
void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    enemy->addEnemy(player->x, player->y, 2*player->radius);
    CheckEndGame();

    //healthbar coordinates to position it in the top right corner
    float hbX = logicSpace.x + logicSpace.width;
    float hbY = logicSpace.y + logicSpace.height - HB->lengthY - HB->lengthY / 2;

    //draw healthbar
    modelMatrix = visMatrix *
        transform2D::Translate(hbX,hbY) *
        glm::mat3(1);
    RenderMesh2D(meshes["health_contour"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix *
        transform2D::Translate(hbX + HB->damageTaken, hbY) *
        glm::mat3(1);
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);

    //draw score bar
    float sbX = hbX;
    float sbY = hbY - SB->lengthY - SB->lengthY / 3;
    modelMatrix = visMatrix *
        transform2D::Translate(sbX, sbY) *
        glm::mat3(1);
    RenderMesh2D(meshes["score_contour"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix *
        transform2D::Translate(sbX, sbY) *
        glm::mat3(1);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    DrawScene(visMatrix, deltaTimeSeconds);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    //check if the player can move up, down, left or right
    bool goUp = true, goDown = true, goLeft = true, goRight = true;
    for (int i = 0; i < obstacles.size(); i++) {
        if (player->CheckCollision(obstacles[i]->edgeUp)) {
            goDown = false;
        }
        if (player->CheckCollision(obstacles[i]->edgeDown)) {
            goUp = false;
        }
        if (player->CheckCollision(obstacles[i]->edgeLeft)) {
            goRight = false;
        }
        if (player->CheckCollision(obstacles[i]->edgeRight)) {
            goLeft = false;
        }
    }

    //respect the boundaries of the map and of the obstacles
    if (!(player->CheckCollision(map->map_edges[0])) && goUp) {
       if (press_w == true) {
            player->y += 200 * deltaTime;
            logicSpace.y += 200 * deltaTime;
       }
    }
    if (!(player->CheckCollision(map->map_edges[1])) && goLeft) {
        if (press_a == true) {
            player->x -= 200 * deltaTime;
            logicSpace.x -= 200 * deltaTime;
        }
    }
    if (!(player->CheckCollision(map->map_edges[2])) && goDown) {
        if (press_s == true) {
            player->y -= 200 * deltaTime;
           logicSpace.y -= 200 * deltaTime;
       }
    }
    if (!(player->CheckCollision(map->map_edges[3])) && goRight) {
        if (press_d == true) {
            player->x += 200 * deltaTime;
            logicSpace.x += 200 * deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_W) {
        press_w = true;
    }
    if (key == GLFW_KEY_A) {
        press_a = true;
    }
    if (key == GLFW_KEY_S) {
        press_s = true;
    }
    if (key == GLFW_KEY_D) {
        press_d = true;
    }
    if (enemy->centerX.size() == 0 && HB->damageTaken <HB->lengthX && key == GLFW_KEY_C) {
        exit(0);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_W) {
        press_w = false;
    }
    if (key == GLFW_KEY_A) {
        press_a = false;
    }
    if (key == GLFW_KEY_S) {
        press_s = false;
    }
    if (key == GLFW_KEY_D) {
        press_d = false;
    }
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //since the player is in the middle of the logic window and the viewSpace window is transformed to the logicWindow
    //that means we have to rotate the object around the center of the viewSpace window
    float middleX = (viewSpace.x + viewSpace.width) / 2;
    float middleY = (viewSpace.y + viewSpace.height) / 2;
    
    float distX = mouseX - middleX;
    float distY = mouseY - middleY;
    double angle = atan2(distX, distY);
    player->rotateAngle = angle;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    getProjectile = true;
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    float deltaTime = (float)(clock() - begin_time);
    if (deltaTime > projectile->fireRate) {
        //recalculate the angle in which the projectile is launched
        float middleX = (viewSpace.x + viewSpace.width) / 2;
        float middleY = (viewSpace.y + viewSpace.height) / 2;

        float distX = mouseX - middleX;
        float distY = mouseY - middleY;
        double angle = atan2(distX, distY);

        projectile->times.push_back(0.0);
        projectile->angles.push_back(angle);
        projectile->playerX.push_back(player->x);
        projectile->playerY.push_back(player->y);

        begin_time = clock();
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
