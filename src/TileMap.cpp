#include "TileMap.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <queue>

bool TileMap::load(const std::string& path, std::string& error)
{
    tiles.clear();
    width = 0;
    height = 0;
    start = Vector2{0.0f, 0.0f};
    spawnList.clear();
    error.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        error = "Could not open level file: " + path;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }

    if (lines.empty()) {
        error = "The level file is empty";
        return false;
    }

    const int parsedCols = static_cast<int>(lines.front().size());
    const int parsedRows = static_cast<int>(lines.size());

    if (parsedCols != cfg::mapCols || parsedRows != cfg::mapRows) {
        error = "The map must be " + std::to_string(cfg::mapCols) + " x " + std::to_string(cfg::mapRows) + " tiles";
        return false;
    }

    for (int row = 0; row < parsedRows; ++row) {
        if (static_cast<int>(lines[static_cast<size_t>(row)].size()) != parsedCols) {
            error = "Line " + std::to_string(row + 1) + " has " + std::to_string(lines[static_cast<size_t>(row)].size()) + " columns, expected " + std::to_string(parsedCols);
            return false;
        }
    }

    std::vector<Tile> parsed(static_cast<size_t>(parsedCols * parsedRows), Tile::Floor);
    std::vector<SpawnPoint> parsedSpawns;
    Vector2 parsedStart{0.0f, 0.0f};
    int startCol = -1;
    int startRow = -1;
    int playerCount = 0;
    int exitCount = 0;
    int keyCount = 0;
    int doorCount = 0;

    for (int row = 0; row < parsedRows; ++row) {
        for (int col = 0; col < parsedCols; ++col) {
            const char symbol = lines[static_cast<size_t>(row)][static_cast<size_t>(col)];
            const size_t index = static_cast<size_t>(row * parsedCols + col);

            switch (symbol) {
            case '#':
                parsed[index] = Tile::Wall;
                break;
            case '.':
                parsed[index] = Tile::Floor;
                break;
            case 'P':
                ++playerCount;
                startCol = col;
                startRow = row;
                parsedStart = Vector2{static_cast<float>(col * cfg::tileSize) + cfg::tileSize / 2.0f,
                                      static_cast<float>(row * cfg::tileSize) + cfg::tileSize / 2.0f};
                break;
            case 'Z':
            case 'S':
            case 'H':
            case 'A':
            case 'K':
                parsedSpawns.push_back(SpawnPoint{symbol, col, row});
                if (symbol == 'K') {
                    ++keyCount;
                }
                break;
            case 'D':
                ++doorCount;
                parsed[index] = Tile::Door;
                break;
            case 'E':
                ++exitCount;
                parsed[index] = Tile::Exit;
                break;
            default:
                error = "Line " + std::to_string(row + 1) + ", column " + std::to_string(col + 1) + ": unknown tile '" + std::string(1, symbol) + "'";
                return false;
            }
        }
    }

    for (int col = 0; col < parsedCols; ++col) {
        if (parsed[static_cast<size_t>(col)] != Tile::Wall ||
            parsed[static_cast<size_t>((parsedRows - 1) * parsedCols + col)] != Tile::Wall) {
            error = "The map must be fully enclosed by walls";
            return false;
        }
    }

    for (int row = 0; row < parsedRows; ++row) {
        if (parsed[static_cast<size_t>(row * parsedCols)] != Tile::Wall ||
            parsed[static_cast<size_t>(row * parsedCols + parsedCols - 1)] != Tile::Wall) {
            error = "The map must be fully enclosed by walls";
            return false;
        }
    }

    if (playerCount != 1) {
        error = "The map must contain exactly one player start";
        return false;
    }

    if (exitCount != 1) {
        error = "The map must contain exactly one exit";
        return false;
    }

    if (keyCount > 1) {
        error = "The map can contain at most one keycard";
        return false;
    }

    if (doorCount > 1) {
        error = "The map can contain at most one door";
        return false;
    }

    if (doorCount == 1 && keyCount == 0) {
        error = "A locked door requires a keycard";
        return false;
    }

    if (doorCount == 1) {
        int keyCol = -1;
        int keyRow = -1;
        for (const SpawnPoint& spawn : parsedSpawns) {
            if (spawn.kind == 'K') {
                keyCol = spawn.col;
                keyRow = spawn.row;
                break;
            }
        }

        std::vector<bool> visited(static_cast<size_t>(parsedCols * parsedRows), false);
        std::queue<int> frontier;
        const int startIndex = startRow * parsedCols + startCol;
        visited[static_cast<size_t>(startIndex)] = true;
        frontier.push(startIndex);
        bool keyReachable = false;

        while (!frontier.empty()) {
            const int index = frontier.front();
            frontier.pop();
            const int col = index % parsedCols;
            const int row = index / parsedCols;

            if (col == keyCol && row == keyRow) {
                keyReachable = true;
                break;
            }

            const int offsets[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            for (const auto& offset : offsets) {
                const int nextCol = col + offset[0];
                const int nextRow = row + offset[1];
                if (nextCol < 0 || nextRow < 0 || nextCol >= parsedCols || nextRow >= parsedRows) {
                    continue;
                }
                const int nextIndex = nextRow * parsedCols + nextCol;
                if (visited[static_cast<size_t>(nextIndex)]) {
                    continue;
                }
                const Tile nextTile = parsed[static_cast<size_t>(nextIndex)];
                if (nextTile == Tile::Wall || nextTile == Tile::Door) {
                    continue;
                }
                visited[static_cast<size_t>(nextIndex)] = true;
                frontier.push(nextIndex);
            }
        }

        if (!keyReachable) {
            error = "The keycard is not reachable without the door";
            return false;
        }
    }

    tiles = parsed;
    width = parsedCols;
    height = parsedRows;
    start = parsedStart;
    spawnList = parsedSpawns;
    return true;
}

Tile TileMap::at(int col, int row) const
{
    if (col < 0 || row < 0 || col >= width || row >= height) {
        return Tile::Wall;
    }
    return tiles[static_cast<size_t>(row * width + col)];
}

bool TileMap::isSolidAt(Vector2 worldPosition) const
{
    const int col = static_cast<int>(std::floor(worldPosition.x / static_cast<float>(cfg::tileSize)));
    const int row = static_cast<int>(std::floor(worldPosition.y / static_cast<float>(cfg::tileSize)));
    const Tile tile = at(col, row);
    return tile == Tile::Wall || tile == Tile::Door;
}

void TileMap::openDoor(int col, int row)
{
    if (col < 0 || row < 0 || col >= width || row >= height) {
        return;
    }
    Tile& tile = tiles[static_cast<size_t>(row * width + col)];
    if (tile == Tile::Door) {
        tile = Tile::Floor;
    }
}

int TileMap::cols() const
{
    return width;
}

int TileMap::rows() const
{
    return height;
}

Vector2 TileMap::playerStart() const
{
    return start;
}

const std::vector<SpawnPoint>& TileMap::spawns() const
{
    return spawnList;
}

void TileMap::draw() const
{
    const Color floorColor{34, 34, 40, 255};
    const Color wallColor{78, 78, 92, 255};
    const Color wallTopColor{102, 102, 118, 255};
    const Color doorColor{188, 146, 46, 255};
    const Color doorLineColor{120, 90, 24, 255};
    const Color exitColor{46, 170, 90, 255};
    const Color exitInnerColor{28, 98, 54, 255};

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            const float x = static_cast<float>(col * cfg::tileSize);
            const float y = static_cast<float>(row * cfg::tileSize);
            const Tile tile = at(col, row);

            if (tile == Tile::Wall) {
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), cfg::tileSize, cfg::tileSize, wallColor);
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), cfg::tileSize, 4, wallTopColor);
            } else if (tile == Tile::Door) {
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), cfg::tileSize, cfg::tileSize, doorColor);
                DrawRectangle(static_cast<int>(x) + 6, static_cast<int>(y) + 4, 4, cfg::tileSize - 8, doorLineColor);
                DrawRectangle(static_cast<int>(x) + cfg::tileSize - 10, static_cast<int>(y) + 4, 4, cfg::tileSize - 8, doorLineColor);
            } else if (tile == Tile::Exit) {
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), cfg::tileSize, cfg::tileSize, floorColor);
                DrawRectangle(static_cast<int>(x) + 4, static_cast<int>(y) + 4, cfg::tileSize - 8, cfg::tileSize - 8, exitColor);
                DrawRectangle(static_cast<int>(x) + 11, static_cast<int>(y) + 11, cfg::tileSize - 22, cfg::tileSize - 22, exitInnerColor);
            } else {
                DrawRectangle(static_cast<int>(x), static_cast<int>(y), cfg::tileSize, cfg::tileSize, floorColor);
            }
        }
    }
}
