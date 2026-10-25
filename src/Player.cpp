#include "Player.h"

#include "Collision.h"

#include <algorithm>
#include <cmath>

void updatePlayer(Player& player, const TileMap& map, std::vector<Bullet>& bullets, float dt)
{
    player.fireCooldown = std::max(0.0f, player.fireCooldown - dt);
    player.hitFlashTimer = std::max(0.0f, player.hitFlashTimer - dt);

    Vector2 movement{0.0f, 0.0f};
    if (IsKeyDown(KEY_W)) {
        movement.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S)) {
        movement.y += 1.0f;
    }
    if (IsKeyDown(KEY_A)) {
        movement.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D)) {
        movement.x += 1.0f;
    }

    const float movementLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (movementLength > 0.0001f) {
        movement.x /= movementLength;
        movement.y /= movementLength;
        const Vector2 delta{movement.x * cfg::playerSpeed * dt, movement.y * cfg::playerSpeed * dt};
        player.position = collision::moveWithCollision(map, player.position, delta, cfg::playerRadius);
    }

    const Vector2 mouse = GetMousePosition();
    const Vector2 aimOffset{mouse.x - player.position.x, mouse.y - player.position.y};
    const float aimLength = std::sqrt(aimOffset.x * aimOffset.x + aimOffset.y * aimOffset.y);
    if (aimLength > 0.0001f) {
        player.aimDirection = Vector2{aimOffset.x / aimLength, aimOffset.y / aimLength};
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player.fireCooldown <= 0.0f && player.ammo > 0) {
        const float muzzleDistance = cfg::playerRadius + cfg::playerBulletRadius + cfg::muzzleOffset;
        const Vector2 muzzle{player.position.x + player.aimDirection.x * muzzleDistance,
                             player.position.y + player.aimDirection.y * muzzleDistance};
        spawnBullet(bullets, muzzle, player.aimDirection, true);
        --player.ammo;
        player.fireCooldown = cfg::playerFireInterval;
    }
}

void drawPlayer(const Player& player)
{
    const Color bodyColor = player.hitFlashTimer > 0.0f ? Color{244, 72, 72, 255} : Color{66, 132, 244, 255};
    const Color outlineColor{18, 26, 48, 255};
    const Vector2 aimEnd{player.position.x + player.aimDirection.x * cfg::aimIndicatorLength,
                         player.position.y + player.aimDirection.y * cfg::aimIndicatorLength};

    DrawCircleV(player.position, cfg::playerRadius, bodyColor);
    DrawCircleLines(static_cast<int>(player.position.x), static_cast<int>(player.position.y), cfg::playerRadius, outlineColor);
    DrawLineEx(player.position, aimEnd, cfg::aimIndicatorThickness, Color{148, 210, 255, 230});
    DrawCircleV(aimEnd, 2.5f, Color{220, 242, 255, 255});

    if (player.fireCooldown > cfg::playerFireInterval - cfg::muzzleFlashDuration) {
        const float muzzleDistance = cfg::playerRadius + cfg::playerBulletRadius + cfg::muzzleOffset;
        const Vector2 muzzle{player.position.x + player.aimDirection.x * muzzleDistance,
                             player.position.y + player.aimDirection.y * muzzleDistance};
        DrawCircleV(muzzle, 5.0f, Color{255, 228, 110, 255});
        DrawCircleV(muzzle, 2.0f, Color{255, 250, 220, 255});
    }
}
