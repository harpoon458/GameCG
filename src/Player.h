#pragma once

#include "Bullet.h"
#include "Config.h"
#include "TileMap.h"
#include "raylib.h"

#include <vector>

struct Player {
    Vector2 position{};
    Vector2 aimDirection{1.0f, 0.0f};
    int health = cfg::playerMaxHealth;
    int ammo = cfg::playerStartingAmmo;
    bool hasKey = false;
    float fireCooldown = 0.0f;
    float hitFlashTimer = 0.0f;
};

void updatePlayer(Player& player, const TileMap& map, std::vector<Bullet>& bullets, float dt);
void drawPlayer(const Player& player);
