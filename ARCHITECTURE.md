# Architecture: Top-Down Demon Shooter

## 1. Folder Layout

```
demon-shooter/
  CMakeLists.txt
  README.md
  requirements.md
  ARCHITECTURE.md
  BUILD_PLAN.md
  PROMPT.md
  REPORT_OUTLINE.md
  levels/
    level1.txt ... level10.txt
  src/
    main.cpp
    Config.h
    Game.h
    Game.cpp
    TileMap.h
    TileMap.cpp
    Collision.h
    Collision.cpp
    Player.h
    Player.cpp
    Enemy.h
    Enemy.cpp
    Bullet.h
    Bullet.cpp
    Pickup.h
    Pickup.cpp
    Hud.h
    Hud.cpp
    Pathfinding.h
    Pathfinding.cpp
```

`Pathfinding.*` is created only if stretch goal ST-02 is chosen.

## 2. Module Responsibilities

| Module | Responsibility | Knows about |
|---|---|---|
| main.cpp | Create `Game` and call `run()` | Game |
| Config.h | Every tunable constant in `namespace cfg` | nothing |
| Game | Window, game states, main loop, owns all entities, level load and reset | everything |
| TileMap | Parse and validate level file, tile queries, door opening, draw map, expose spawn list | Config |
| Collision | Circle vs tile movement, line of sight, circle vs circle test | TileMap, Config |
| Player | Input to movement and aim, firing cooldown, ammo, health, key flag, draw | TileMap, Collision, Bullet |
| Enemy | Chaser and Shooter behavior, state machine, damage, draw | TileMap, Collision, Bullet |
| Bullet | Projectile movement and lifetime, draw | TileMap |
| Pickup | Pickup data, touch test, draw | Config |
| Hud | Draw health bar, ammo, key, kills, time, and overlay screens | Config |
| Pathfinding | BFS over the tile grid (stretch) | TileMap |

Dependency rule: lower modules never include `Game.h`. `Game` is the only place where modules meet.

## 3. Core Data Types

```cpp
enum class GameState { Menu, Playing, Paused, GameOver, LevelComplete, AllLevelsComplete };

enum class Tile { Floor, Wall, Door, Exit };

struct SpawnPoint {
    char kind;
    int col;
    int row;
};

enum class EnemyType { Chaser, Shooter };
enum class EnemyState { Idle, Chase, Attack, Engage };

struct Enemy {
    EnemyType type;
    EnemyState state;
    Vector2 position;
    float radius;
    int health;
    float attackCooldown;
    float lostSightTimer;
    float hitFlashTimer;
    bool alive;
};

struct Bullet {
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius;
    float lifetime;
    int damage;
    bool fromPlayer;
    bool alive;
};

enum class PickupType { Health, Ammo, Key };

struct Pickup {
    PickupType type;
    Vector2 position;
    bool collected;
};

struct Player {
    Vector2 position;
    Vector2 aimDirection;
    int health;
    int ammo;
    bool hasKey;
    float fireCooldown;
    float hitFlashTimer;
};
```

## 4. Key Interfaces

```cpp
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
};

namespace collision {
Vector2 moveWithCollision(const TileMap& map, Vector2 position, Vector2 delta, float radius);
bool lineOfSight(const TileMap& map, Vector2 from, Vector2 to);
bool circlesOverlap(Vector2 a, float radiusA, Vector2 b, float radiusB);
}

class Game {
public:
    void run();
private:
    bool loadLevel(int level, bool resetCampaign);
    void startNewGame();
    void restartLevel();
    void advanceLevel();
    void handleInput();
    void update(float dt);
    void draw();
    GameState state;
    TileMap map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Pickup> pickups;
    int currentLevel;
    int levelKills;
    float levelTime;
    int totalKills;
    float totalTime;
    std::string levelError;
};
```

Free functions for entity logic (`updatePlayer`, `updateEnemy`, `updateBullet`, `drawEnemy`, and so on) live in the entity's own file pair. Plain structs plus free functions are used instead of inheritance to keep the code easy to explain.

## 5. Main Loop and Update Order

```
while window is open
    dt = min(GetFrameTime(), 1/30)
    handleInput()
    if state is Playing
        update(dt)
    draw()
```

`update(dt)` runs in this fixed order every frame:

1. Update timers (elapsed time, cooldowns, flash timers)
2. Player: read movement and aim, move with collision, fire if allowed
3. Enemies: update state machine, move with collision, attack
4. Bullets: advance, check lifetime and walls
5. Collisions: player bullets vs enemies, enemy projectiles vs player, chaser contact vs player
6. Pickups: touch test against the player
7. Door and exit: door touch with key, exit touch
8. Cleanup: erase dead enemies and bullets, collected pickups
9. State check: health 0 gives GameOver, exit touched gives LevelComplete

## 6. Rendering Order

1. Map (floor, walls, door, exit)
2. Pickups
3. Enemies
4. Bullets
5. Player and aim indicator
6. HUD bar
7. State overlay (dimmed rectangle plus centered text)

## 7. Collision Design

### 7.1 Player and enemy movement
Move on the X axis first, resolve, then move on the Y axis, resolve. This gives sliding along walls for free.

Resolve circle against tiles:
1. Compute the tile range covered by the circle's bounding box
2. For each solid tile in range, find the closest point on the tile rectangle to the circle center
3. If the distance to that point is less than the radius, push the circle out along the vector from the point to the center by `radius - distance`

### 7.2 Bullets
A bullet moves at most 600 px/s x (1/30 s) = 20 px per frame, below the 32 px tile size, so testing the tile at the bullet position is enough. Do not raise speed or the dt clamp without adding sub-steps.

### 7.3 Hit tests
Bullet vs enemy, enemy projectile vs player, pickup vs player, and chaser contact all use circle vs circle.

## 8. Line of Sight

`lineOfSight(map, from, to)` walks from `from` to `to` in steps of `tileSize / 4`. If any sample point lies inside a solid tile, the line is blocked. Doors count as solid while closed.

## 9. Enemy AI

### 9.1 Chaser

```
Idle --(player within sight range AND line of sight)--> Chase
Chase --(touching player)--> Attack
Attack --(no longer touching)--> Chase
Chase --(no line of sight for 2 s)--> Idle
Idle --(hit by bullet)--> Chase
```

- Chase: move along the normalized vector toward the player using `moveWithCollision`
- Attack: deal contact damage when `attackCooldown` reaches 0, then reset it to 0.8 s

### 9.2 Shooter

```
Idle --(player within sight range AND line of sight)--> Engage
Engage --(no line of sight for 2 s)--> Idle
Idle --(hit by bullet)--> Engage
```

In Engage:
- distance below 180: move away from the player
- distance above 260: move toward the player
- otherwise hold position
- when `attackCooldown` is 0 and line of sight exists: spawn an enemy projectile aimed at the player, reset cooldown to 1.5 s

### 9.3 Known limitation
Without pathfinding, enemies walk straight at the player and can get stuck behind wall corners. This is acceptable for the base game and is the motivation for ST-02.

## 10. Level Loading Flow

```
Game::loadLevel(path)
    map.load(path, error)
        read file lines
        validate rectangular shape, enclosure, symbol counts
        build tile grid, record spawns and player start
    on failure: store error, stay in Menu, draw the error text
    on success:
        reset player at playerStart
        for each spawn: create Enemy, Pickup according to kind
        reset kills, elapsedTime, bullets
```

`resetLevel()` calls `loadLevel` again with the same path, which guarantees a full reset.

## 11. Optional BFS Pathfinding (ST-02)

```cpp
namespace pathfinding {
std::vector<Vector2> findPath(const TileMap& map, Vector2 from, Vector2 to);
}
```

- Breadth-first search over the 4-connected tile grid, treating walls and closed doors as blocked
- Returns tile-center waypoints from start to goal
- A chaser recomputes its path every 0.3 s and walks toward the first waypoint
- Complexity: O(cols x rows) time and space per search, at most 512 tiles for the sample level
