#include "Collision.h"

#include <algorithm>
#include <cmath>

namespace {

void resolveCircleAgainstTile(Vector2& position, float radius, int col, int row)
{
    const float left = static_cast<float>(col * cfg::tileSize);
    const float top = static_cast<float>(row * cfg::tileSize);
    const float right = left + static_cast<float>(cfg::tileSize);
    const float bottom = top + static_cast<float>(cfg::tileSize);

    const float closestX = std::clamp(position.x, left, right);
    const float closestY = std::clamp(position.y, top, bottom);
    const float dx = position.x - closestX;
    const float dy = position.y - closestY;
    const float distanceSquared = dx * dx + dy * dy;

    if (distanceSquared > 0.000001f) {
        if (distanceSquared >= radius * radius) {
            return;
        }
        const float distance = std::sqrt(distanceSquared);
        const float push = radius - distance;
        position.x += dx / distance * push;
        position.y += dy / distance * push;
        return;
    }

    const float toLeft = position.x - left;
    const float toRight = right - position.x;
    const float toTop = position.y - top;
    const float toBottom = bottom - position.y;

    float smallest = toLeft;
    int side = 0;
    if (toRight < smallest) {
        smallest = toRight;
        side = 1;
    }
    if (toTop < smallest) {
        smallest = toTop;
        side = 2;
    }
    if (toBottom < smallest) {
        side = 3;
    }

    if (side == 0) {
        position.x = left - radius;
    } else if (side == 1) {
        position.x = right + radius;
    } else if (side == 2) {
        position.y = top - radius;
    } else {
        position.y = bottom + radius;
    }
}

void resolveAgainstTiles(const TileMap& map, Vector2& position, float radius)
{
    const int minCol = static_cast<int>(std::floor((position.x - radius) / static_cast<float>(cfg::tileSize)));
    const int maxCol = static_cast<int>(std::floor((position.x + radius) / static_cast<float>(cfg::tileSize)));
    const int minRow = static_cast<int>(std::floor((position.y - radius) / static_cast<float>(cfg::tileSize)));
    const int maxRow = static_cast<int>(std::floor((position.y + radius) / static_cast<float>(cfg::tileSize)));

    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            const Tile tile = map.at(col, row);
            if (tile == Tile::Wall || tile == Tile::Door) {
                resolveCircleAgainstTile(position, radius, col, row);
            }
        }
    }
}

}

namespace collision {

Vector2 moveWithCollision(const TileMap& map, Vector2 position, Vector2 delta, float radius)
{
    Vector2 result = position;
    result.x += delta.x;
    resolveAgainstTiles(map, result, radius);
    result.y += delta.y;
    resolveAgainstTiles(map, result, radius);
    return result;
}

bool lineOfSight(const TileMap& map, Vector2 from, Vector2 to)
{
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;
    const float distance = std::sqrt(dx * dx + dy * dy);
    if (distance < 0.0001f) {
        return true;
    }

    const int steps = std::max(1, static_cast<int>(std::ceil(distance / cfg::lineOfSightStep)));
    for (int step = 0; step <= steps; ++step) {
        const float t = static_cast<float>(step) / static_cast<float>(steps);
        const Vector2 sample{from.x + dx * t, from.y + dy * t};
        if (map.isSolidAt(sample)) {
            return false;
        }
    }
    return true;
}

bool circlesOverlap(Vector2 a, float radiusA, Vector2 b, float radiusB)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float radiusSum = radiusA + radiusB;
    return dx * dx + dy * dy <= radiusSum * radiusSum;
}

bool circleTouchesTile(Vector2 center, float radius, int col, int row)
{
    const float left = static_cast<float>(col * cfg::tileSize);
    const float top = static_cast<float>(row * cfg::tileSize);
    const float right = left + static_cast<float>(cfg::tileSize);
    const float bottom = top + static_cast<float>(cfg::tileSize);
    const float closestX = std::clamp(center.x, left, right);
    const float closestY = std::clamp(center.y, top, bottom);
    const float dx = center.x - closestX;
    const float dy = center.y - closestY;
    return dx * dx + dy * dy <= radius * radius;
}

}
