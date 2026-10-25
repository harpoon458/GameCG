#include "Pickup.h"

#include "Collision.h"
#include "Config.h"

Pickup makePickup(PickupType type, Vector2 position)
{
    Pickup pickup;
    pickup.type = type;
    pickup.position = position;
    pickup.collected = false;
    return pickup;
}

bool pickupTouchesPlayer(const Pickup& pickup, Vector2 playerPosition, float playerRadius)
{
    if (pickup.collected) {
        return false;
    }
    return collision::circlesOverlap(pickup.position, cfg::pickupRadius, playerPosition, playerRadius);
}

void drawPickup(const Pickup& pickup)
{
    if (pickup.collected) {
        return;
    }

    const float x = pickup.position.x;
    const float y = pickup.position.y;

    if (pickup.type == PickupType::Health) {
        DrawCircleV(pickup.position, cfg::pickupDrawRadius, Color{76, 206, 118, 255});
        DrawRectangle(static_cast<int>(x) - 2, static_cast<int>(y) - 6, 4, 12, RAYWHITE);
        DrawRectangle(static_cast<int>(x) - 6, static_cast<int>(y) - 2, 12, 4, RAYWHITE);
    } else if (pickup.type == PickupType::Ammo) {
        DrawRectangle(static_cast<int>(x) - 8, static_cast<int>(y) - 8, 16, 16, Color{226, 182, 52, 255});
        DrawRectangle(static_cast<int>(x) - 5, static_cast<int>(y) - 5, 10, 10, Color{86, 64, 18, 255});
        DrawRectangle(static_cast<int>(x) - 1, static_cast<int>(y) - 4, 2, 8, Color{246, 226, 140, 255});
    } else {
        DrawCircleV(Vector2{x - 3.0f, y}, 5.0f, Color{244, 204, 64, 255});
        DrawRectangle(static_cast<int>(x) - 3, static_cast<int>(y) - 2, 11, 4, Color{244, 204, 64, 255});
        DrawRectangle(static_cast<int>(x) + 5, static_cast<int>(y) + 1, 3, 4, Color{244, 204, 64, 255});
    }
}
