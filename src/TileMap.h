#pragma once

#include "Config.h"
#include "raylib.h"

#include <string>
#include <vector>

enum class Tile { Floor, Wall, Door, Exit };

struct SpawnPoint {
    char kind;
    int col;
    int row;
};

class TileMap {
public:
    bool load(const std::string& path, std::string& error);
    Tile at(int col, int row) const;
    bool isSolidAt(Vector2 worldPosition) const;
    void openDoor(int col, int row);
    int cols() const;
    int rows() const;
    Vector2 playerStart() const;
    const std::vector<SpawnPoint>& spawns() const;
    void draw() const;

private:
    std::vector<Tile> tiles;
    int width = 0;
    int height = 0;
    Vector2 start{};
    std::vector<SpawnPoint> spawnList;
};
