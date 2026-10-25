#pragma once

#include "TileMap.h"
#include "raylib.h"

namespace collision {

Vector2 moveWithCollision(const TileMap& map, Vector2 position, Vector2 delta, float radius);
bool lineOfSight(const TileMap& map, Vector2 from, Vector2 to);
bool circlesOverlap(Vector2 a, float radiusA, Vector2 b, float radiusB);
bool circleTouchesTile(Vector2 center, float radius, int col, int row);

}
