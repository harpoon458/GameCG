#pragma once

#include "Bullet.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Player.h"
#include "TileMap.h"

#include <string>
#include <vector>

enum class GameState { Menu, Playing, Paused, GameOver, LevelComplete, AllLevelsComplete };

class Game {
public:
    void run();

private:
    bool loadLevel(int level, bool resetCampaign);
    void startNewGame();
    void restartLevel();
    void advanceLevel();
    void handleInput();
    void update(float dt);
    void updateEnemies(float dt);
    void handleCombat();
    void handlePickups();
    bool handleDoorAndExit();
    void cleanup();
    void draw();

    GameState state = GameState::Menu;
    TileMap map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Pickup> pickups;
    int currentLevel = 1;
    int levelKills = 0;
    float levelTime = 0.0f;
    int totalKills = 0;
    float totalTime = 0.0f;
    std::string levelError;
    bool quitRequested = false;
};
