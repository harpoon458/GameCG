#include "Bullet.h"

#include "Config.h"

#include <cmath>

void spawnBullet(std::vector<Bullet>& bullets, Vector2 position, Vector2 direction, bool fromPlayer)
{
    Bullet bullet;
    bullet.position = position;

    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0001f) {
        bullet.direction = Vector2{direction.x / length, direction.y / length};
    } else {
        bullet.direction = Vector2{1.0f, 0.0f};
    }

    if (fromPlayer) {
        bullet.speed = cfg::playerBulletSpeed;
        bullet.radius = cfg::playerBulletRadius;
        bullet.lifetime = cfg::playerBulletLifetime;
        bullet.damage = cfg::playerBulletDamage;
    } else {
        bullet.speed = cfg::enemyBulletSpeed;
        bullet.radius = cfg::enemyBulletRadius;
        bullet.lifetime = cfg::enemyBulletLifetime;
        bullet.damage = cfg::enemyBulletDamage;
    }

    bullet.fromPlayer = fromPlayer;
    bullet.alive = true;
    bullets.push_back(bullet);
}

void updateBullet(Bullet& bullet, const TileMap& map, float dt)
{
    if (!bullet.alive) {
        return;
    }

    bullet.lifetime -= dt;
    if (bullet.lifetime <= 0.0f) {
        bullet.alive = false;
        return;
    }

    bullet.position.x += bullet.direction.x * bullet.speed * dt;
    bullet.position.y += bullet.direction.y * bullet.speed * dt;

    if (map.isSolidAt(bullet.position)) {
        bullet.alive = false;
    }
}

void drawBullet(const Bullet& bullet)
{
    if (!bullet.alive) {
        return;
    }
    const Color color = bullet.fromPlayer ? Color{255, 222, 74, 255} : Color{190, 82, 232, 255};
    DrawCircleV(bullet.position, bullet.radius, color);
}
