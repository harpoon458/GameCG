#pragma once

#include "raylib.h"

enum class PickupType { Health, Ammo, Key };

struct Pickup {
    PickupType type = PickupType::Health;
    Vector2 position{};
    bool collected = false;
};

Pickup makePickup(PickupType type, Vector2 position);
bool pickupTouchesPlayer(const Pickup& pickup, Vector2 playerPosition, float playerRadius);
void drawPickup(const Pickup& pickup);
