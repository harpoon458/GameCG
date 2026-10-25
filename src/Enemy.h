#pragma once

#include "Bullet.h"
#include "Player.h"
#include "TileMap.h"
#include "raylib.h"

#include <vector>

enum class EnemyType { Chaser, Shooter };
enum class EnemyState { Idle, Chase, Attack, Engage };

struct Enemy {
    EnemyType type = EnemyType::Chaser;
    EnemyState state = EnemyState::Idle;
    Vector2 position{};
    float radius = cfg::chaserRadius;
    int health = cfg::chaserHealth;
    float attackCooldown = 0.0f;
    float lostSightTimer = 0.0f;
    float hitFlashTimer = 0.0f;
    bool alive = true;
};

Enemy makeEnemy(EnemyType type, Vector2 position);
void updateEnemy(Enemy& enemy, const Player& player, const TileMap& map, std::vector<Bullet>& bullets, float dt);
bool damageEnemy(Enemy& enemy, int damage);
void separateEnemies(std::vector<Enemy>& enemies, const TileMap& map);
void drawEnemy(const Enemy& enemy);
