#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Player.h"
#include "Map.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "Enemy.h"
#include "HealthBar.h"
#include "ScoreBar.h"
#include "Time.h"
#include <vector>
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        //clamps a value in a given range
        float clamp(float value, float min, float max) {
            return std::max(min, std::min(max, value));
        }

        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void DisplayEnemy(glm::mat3 visMatrix, float deltaTimeSeconds);
        int checkMapCollisionsForEnemies(int i, float angle);
        void moveEnemy(int i, float angle);
        bool CheckCollision(int enemy, Object* obj);
        bool CheckCollisionProjectile(int enemy, int p);
        void CheckEndGame();
        void initializeMapEdges();
        void initializeObstacleEdges();

    protected:
        std::clock_t begin_time;
        float cx, cy;
        glm::mat3 modelMatrix, visMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        std::vector<Obstacle*> obstacles = std::vector<Obstacle*>(5);
        Enemy* enemy;
        bool getProjectile = false;
        bool gameWon = false;
        Player* player;
        Map* map;
        HealthBar* HB;
        ScoreBar* SB;
        bool press_w = false,
            press_a = false,
            press_s = false,
            press_d = false;
        Projectile* projectile;
    };
}   // namespace m1
