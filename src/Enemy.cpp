#include "Enemy.h"

#include "Collision.h"

#include <algorithm>
#include <cmath>

namespace {

void moveEnemy(Enemy& enemy, Vector2 direction, float speed, const TileMap& map, float dt)
{
    const Vector2 delta{direction.x * speed * dt, direction.y * speed * dt};
    enemy.position = collision::moveWithCollision(map, enemy.position, delta, enemy.radius);
}

}

Enemy makeEnemy(EnemyType type, Vector2 position)
{
    Enemy enemy;
    enemy.type = type;
    enemy.state = EnemyState::Idle;
    enemy.position = position;

    if (type == EnemyType::Chaser) {
        enemy.radius = cfg::chaserRadius;
        enemy.health = cfg::chaserHealth;
    } else {
        enemy.radius = cfg::shooterRadius;
        enemy.health = cfg::shooterHealth;
    }

    enemy.attackCooldown = 0.0f;
    enemy.lostSightTimer = 0.0f;
    enemy.hitFlashTimer = 0.0f;
    enemy.alive = true;
    return enemy;
}

void updateEnemy(Enemy& enemy, const Player& player, const TileMap& map, std::vector<Bullet>& bullets, float dt)
{
    enemy.attackCooldown = std::max(0.0f, enemy.attackCooldown - dt);
    enemy.hitFlashTimer = std::max(0.0f, enemy.hitFlashTimer - dt);

    const Vector2 toPlayer{player.position.x - enemy.position.x, player.position.y - enemy.position.y};
    const float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
    const float sightRange = enemy.type == EnemyType::Chaser ? cfg::chaserSightRange : cfg::shooterSightRange;
    const bool seesPlayer = distance <= sightRange && collision::lineOfSight(map, enemy.position, player.position);

    if (enemy.type == EnemyType::Chaser) {
        switch (enemy.state) {
        case EnemyState::Idle:
            if (seesPlayer) {
                enemy.state = EnemyState::Chase;
                enemy.lostSightTimer = 0.0f;
            }
            break;
        case EnemyState::Chase: {
            const float touchDistance = enemy.radius + cfg::playerRadius;
            if (distance <= touchDistance) {
                enemy.state = EnemyState::Attack;
                enemy.lostSightTimer = 0.0f;
                break;
            }
            if (distance > 0.0001f) {
                moveEnemy(enemy, Vector2{toPlayer.x / distance, toPlayer.y / distance}, cfg::chaserSpeed, map, dt);
            }
            enemy.lostSightTimer = seesPlayer ? 0.0f : enemy.lostSightTimer + dt;
            if (enemy.lostSightTimer >= cfg::enemyLostSightDelay) {
                enemy.state = EnemyState::Idle;
                enemy.lostSightTimer = 0.0f;
            }
            break;
        }
        case EnemyState::Attack: {
            const float breakDistance = enemy.radius + cfg::playerRadius + cfg::chaserTouchMargin * 2.0f;
            if (distance > breakDistance) {
                enemy.state = EnemyState::Chase;
            }
            break;
        }
        case EnemyState::Engage:
            enemy.state = EnemyState::Idle;
            break;
        }
        return;
    }

    switch (enemy.state) {
    case EnemyState::Idle:
        if (seesPlayer) {
            enemy.state = EnemyState::Engage;
            enemy.lostSightTimer = 0.0f;
        }
        break;
    case EnemyState::Engage:
        enemy.lostSightTimer = seesPlayer ? 0.0f : enemy.lostSightTimer + dt;
        if (enemy.lostSightTimer >= cfg::enemyLostSightDelay) {
            enemy.state = EnemyState::Idle;
            enemy.lostSightTimer = 0.0f;
            break;
        }
        if (distance > 0.0001f) {
            if (distance < cfg::shooterPreferredMin) {
                moveEnemy(enemy, Vector2{-toPlayer.x / distance, -toPlayer.y / distance}, cfg::shooterSpeed, map, dt);
            } else if (distance > cfg::shooterPreferredMax) {
                moveEnemy(enemy, Vector2{toPlayer.x / distance, toPlayer.y / distance}, cfg::shooterSpeed, map, dt);
            }
        }
        if (seesPlayer && enemy.attackCooldown <= 0.0f && distance > 0.0001f) {
            const float muzzleDistance = enemy.radius + cfg::enemyBulletRadius + cfg::muzzleOffset;
            const Vector2 direction{toPlayer.x / distance, toPlayer.y / distance};
            const Vector2 muzzle{enemy.position.x + direction.x * muzzleDistance,
                                 enemy.position.y + direction.y * muzzleDistance};
            spawnBullet(bullets, muzzle, direction, false);
            enemy.attackCooldown = cfg::shooterFireInterval;
        }
        break;
    case EnemyState::Chase:
    case EnemyState::Attack:
        enemy.state = EnemyState::Idle;
        break;
    }
}

bool damageEnemy(Enemy& enemy, int damage)
{
    if (!enemy.alive) {
        return false;
    }

    enemy.health -= damage;
    enemy.hitFlashTimer = cfg::enemyHitFlashDuration;

    if (enemy.health <= 0) {
        enemy.health = 0;
        enemy.alive = false;
        return true;
    }

    enemy.state = enemy.type == EnemyType::Chaser ? EnemyState::Chase : EnemyState::Engage;
    enemy.lostSightTimer = 0.0f;
    return false;
}

void separateEnemies(std::vector<Enemy>& enemies, const TileMap& map)
{
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) {
            continue;
        }
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            if (!enemies[j].alive) {
                continue;
            }

            Enemy& first = enemies[i];
            Enemy& second = enemies[j];
            const float dx = second.position.x - first.position.x;
            const float dy = second.position.y - first.position.y;
            const float minimumDistance = first.radius + second.radius;
            const float distanceSquared = dx * dx + dy * dy;
            if (distanceSquared >= minimumDistance * minimumDistance) {
                continue;
            }

            const float distance = std::sqrt(distanceSquared);
            Vector2 normal{1.0f, 0.0f};
            if (distance > 0.0001f) {
                normal = Vector2{dx / distance, dy / distance};
            }

            const float overlap = minimumDistance - distance;
            const float push = overlap * cfg::enemySeparationFactor;
            first.position = collision::moveWithCollision(map, first.position, Vector2{-normal.x * push, -normal.y * push}, first.radius);
            second.position = collision::moveWithCollision(map, second.position, Vector2{normal.x * push, normal.y * push}, second.radius);
        }
    }
}

void drawEnemy(const Enemy& enemy)
{
    if (!enemy.alive) {
        return;
    }

    Color color = enemy.type == EnemyType::Chaser ? Color{222, 62, 62, 255} : Color{240, 150, 52, 255};
    if (enemy.hitFlashTimer > 0.0f) {
        color = Color{255, 255, 255, 255};
    }

    DrawCircleV(enemy.position, enemy.radius, color);
    DrawCircleLines(static_cast<int>(enemy.position.x), static_cast<int>(enemy.position.y), enemy.radius, Color{38, 22, 22, 255});
    if (enemy.type == EnemyType::Shooter) {
        DrawCircleV(enemy.position, enemy.radius * 0.4f, Color{120, 66, 18, 255});
    }
}
