#pragma once

#include "TileMap.h"
#include "raylib.h"

#include <vector>

struct Bullet {
    Vector2 position{};
    Vector2 direction{1.0f, 0.0f};
    float speed = 0.0f;
    float radius = 0.0f;
    float lifetime = 0.0f;
    int damage = 0;
    bool fromPlayer = false;
    bool alive = false;
};

void spawnBullet(std::vector<Bullet>& bullets, Vector2 position, Vector2 direction, bool fromPlayer);
void updateBullet(Bullet& bullet, const TileMap& map, float dt);
void drawBullet(const Bullet& bullet);
