# Requirements: Top-Down Demon Shooter

## 1. Overview

A single-player 2D top-down shooter inspired by classic Doom. The player is a space marine who fights through a small dungeon of demons, collects health and ammo, finds a keycard, unlocks a door, and reaches the exit.

Project type: university assignment.
Priority: a complete, working and explainable game matters more than a feature-heavy one.

## 2. Scope

### 2.1 In scope
- One playable level loaded from a text file (more levels are a stretch goal)
- One weapon (pistol) and two enemy types (Chaser, Shooter)
- Health, ammo and key pickups
- One locked door and one exit
- HUD, menu, pause, game over and level complete screens

### 2.2 Out of scope
- Networking or multiplayer
- 3D rendering or raycasting (see Stretch Goals)
- Save/load system, level editor, inventory
- External art assets (shapes and colors only; audio is optional)

## 3. Technical Constraints

| Item | Decision |
|---|---|
| Language | C++17 |
| Library | raylib 5.x |
| Build system | CMake 3.16+ |
| Platform | Linux (Windows and macOS should also work) |
| Window | 1024 x 576, 60 FPS, title "Demon Shooter" |
| Tile size | 32 x 32 pixels |
| Map area | 32 x 16 tiles (1024 x 512 px) plus a 64 px HUD bar at the bottom |
| Coding style | No comments in source code, meaningful names, all constants in Config.h |

## 4. Controls

| Input | Action |
|---|---|
| W A S D | Move |
| Mouse | Aim |
| Left mouse button (hold) | Fire |
| Esc | Pause / resume (quits from the menu) |
| Enter | Start game from the menu |
| R | Restart level (from Paused, Game Over, Level Complete) |
| 1 / 2 | Switch weapon (stretch goal only) |

## 5. Game Rules

- Win: the player touches the exit tile.
- Lose: player health reaches 0.
- The door is solid until the player holding the keycard touches it. After that it stays open.
- The keycard is not consumed.
- Killing enemies is optional. Kills are only counted for the score screen.
- Enemies do not drop items.

## 6. Functional Requirements

Priority: M = must, S = should, C = could.

| ID | Requirement | Pri |
|---|---|---|
| FR-01 | Open a 1024 x 576 window at 60 FPS with the title "Demon Shooter" | M |
| FR-02 | Load the level from a text file at startup using the format in section 8 | M |
| FR-03 | Render walls, floor, door, exit as colored rectangles | M |
| FR-04 | Move the player with WASD in 8 directions; diagonal speed equals straight speed | M |
| FR-05 | Block the player against walls and closed doors with axis-separated collision (sliding along walls, no tunneling, no sticking) | M |
| FR-06 | Aim at the mouse cursor and draw a facing indicator | M |
| FR-07 | Fire a bullet toward the cursor on left click, rate limited, costing 1 ammo; do nothing at 0 ammo | M |
| FR-08 | Bullets travel straight and are removed on wall hit, enemy hit, or lifetime expiry | M |
| FR-09 | Chaser enemy: idle until the player is in sight range with clear line of sight, then chase and deal contact damage with a cooldown | M |
| FR-10 | Shooter enemy: keep a preferred distance and fire projectiles at the player at intervals while in range with line of sight | M |
| FR-11 | Enemies have health, die at 0, are removed, and increase the kill counter | M |
| FR-12 | Player has health; at 0 the game enters Game Over | M |
| FR-13 | Health and ammo pickups apply on touch, clamp to maximum, and disappear | M |
| FR-14 | Keycard pickup sets a "has key" flag shown on the HUD | M |
| FR-15 | Door opens permanently when the player with the key touches it | M |
| FR-16 | Reaching the exit enters Level Complete | M |
| FR-17 | Implement states Menu, Playing, Paused, GameOver, LevelComplete with the transitions in section 9 | M |
| FR-18 | HUD shows health bar, ammo, key indicator, kills, elapsed time | M |
| FR-19 | Enemies push apart so they do not stack on one point | S |
| FR-20 | Hit feedback: brief white flash on damaged enemies and a red flash on the player | S |
| FR-21 | Enemies woken by a bullet hit switch to chase/engage even without line of sight | S |
| FR-22 | Invalid level files produce a readable on-screen error instead of a crash | S |
| FR-23 | Muzzle flash and light screen shake on firing | C |
| FR-24 | Sound effects for shot, hit, pickup, door | C |

## 7. Entity Specifications

| Property | Player | Chaser | Shooter |
|---|---|---|---|
| Radius (px) | 12 | 12 | 12 |
| Max health | 100 | 50 | 40 |
| Speed (px/s) | 200 | 110 | 70 |
| Sight range (px) | n/a | 280 | 400 |
| Attack | Pistol bullet, 25 damage | Contact, 10 damage every 0.8 s | Projectile, 8 damage every 1.5 s |
| Preferred distance (px) | n/a | touching | 180 to 260 |
| Suggested color | BLUE | RED | ORANGE |

| Projectile | Speed (px/s) | Radius | Lifetime (s) | Damage | Color |
|---|---|---|---|---|---|
| Player bullet | 600 | 3 | 1.2 | 25 | YELLOW |
| Enemy projectile | 260 | 4 | 3.0 | 8 | PURPLE |

| Item | Value |
|---|---|
| Pistol fire interval | 0.25 s (4 shots per second) |
| Starting ammo | 30 |
| Maximum ammo | 99 |
| Health pack | +25 health, clamped to 100 |
| Ammo pack | +12 ammo, clamped to 99 |
| Pickup radius | 10 px |
| Keycard | Gold, no stack, not consumed |

## 8. Level File Format

Plain text file. One character per tile, one line per row, no spaces.

| Char | Meaning |
|---|---|
| `#` | Wall |
| `.` | Floor |
| `P` | Player start (floor) |
| `Z` | Chaser spawn (floor) |
| `S` | Shooter spawn (floor) |
| `H` | Health pack (floor) |
| `A` | Ammo pack (floor) |
| `K` | Keycard (floor) |
| `D` | Locked door |
| `E` | Exit |

Validation rules:
- All rows have the same length.
- The map is fully enclosed by walls.
- Exactly one `P` and exactly one `E`.
- At most one `K` and at most one `D`. If `D` exists, `K` must exist and be reachable without crossing `D`.
- Any other character is an error that reports the line and column.

Provided sample: `levels/level1.txt` (32 x 16 tiles).

## 9. Game States and Transitions

| From | Event | To |
|---|---|---|
| Menu | Enter | Playing |
| Menu | Esc | Quit |
| Playing | Esc | Paused |
| Paused | Esc | Playing |
| Paused | R | Playing (level reset) |
| Playing | Player health 0 | GameOver |
| Playing | Player touches exit | LevelComplete |
| GameOver | R | Playing (level reset) |
| LevelComplete | R | Playing (level reset) |
| GameOver, LevelComplete | Esc | Menu |

All simulation (movement, timers, AI, bullets) is frozen outside the Playing state.

## 10. HUD Layout

Bottom bar, 64 px high, dark background.

- Left: health bar with number
- Center-left: ammo count and key icon (visible only when the key is held)
- Right: kills and elapsed time as mm:ss

Overlay screens (Menu, Paused, GameOver, LevelComplete) draw centered text over a dimmed game view. Level Complete shows kills and time.

## 11. Non-Functional Requirements

| ID | Requirement |
|---|---|
| NFR-01 | Stable 60 FPS with all level entities alive |
| NFR-02 | Frame-rate independent movement and timers using GetFrameTime(), clamped to 1/30 s to prevent tunneling |
| NFR-03 | No raw `new` or `delete`; use value types and `std::vector` |
| NFR-04 | Compiles with no warnings under `-Wall -Wextra` |
| NFR-05 | No comments in source code; names carry the meaning |
| NFR-06 | All tunable numbers live in `Config.h` under `namespace cfg` |
| NFR-07 | One class or concern per file pair as defined in ARCHITECTURE.md |
| NFR-08 | No crash on missing or malformed level file |

## 12. Acceptance Criteria

- [ ] AC-01 The window opens at 1024 x 576 and shows the menu
- [ ] AC-02 Enter loads level1 and shows the full map with the HUD
- [ ] AC-03 The player moves in 8 directions and diagonal movement is not faster
- [ ] AC-04 The player cannot pass walls, slides along them, and never sticks in corners
- [ ] AC-05 The player faces the mouse cursor at all times
- [ ] AC-06 Holding left click fires about 4 bullets per second, each costing 1 ammo, and nothing fires at 0 ammo
- [ ] AC-07 Bullets vanish on walls and enemies
- [ ] AC-08 A Chaser stays idle out of sight, chases when it sees the player, and damages no faster than every 0.8 s
- [ ] AC-09 A Shooter holds its distance and its projectiles hurt the player
- [ ] AC-10 Enemies die after enough damage and the kill count increases
- [ ] AC-11 Pickups restore values, never exceed the maximum, and disappear
- [ ] AC-12 The door is solid without the key and opens on touch with the key
- [ ] AC-13 Reaching the exit shows Level Complete with kills and time
- [ ] AC-14 Health reaching 0 shows Game Over
- [ ] AC-15 Esc pauses and resumes, with everything frozen while paused
- [ ] AC-16 R resets the level completely (player, enemies, pickups, door, timer)
- [ ] AC-17 A broken level file shows an error message instead of crashing
- [ ] AC-18 The game holds at least 55 FPS with all enemies alive
- [ ] AC-19 The build shows no warnings under `-Wall -Wextra`
- [ ] AC-20 No comments exist anywhere in the source

## 13. Stretch Goals

Pick only after all must-have requirements pass.

| ID | Goal | Value for the report |
|---|---|---|
| ST-01 | Shotgun (5 pellets, 15 degree spread, 12 damage each, keys 1 and 2 to switch) | Vector math, weapon abstraction |
| ST-02 | BFS pathfinding so chasers navigate around walls | Graph algorithms, complexity analysis |
| ST-03 | Three levels loaded in sequence from `levels/` | Data-driven design |
| ST-04 | Boss enemy with phases | State machines |
| ST-05 | Minimap | Coordinate transforms |
| ST-06 | Sound effects | Event-driven feedback |
| ST-07 | Fog of war or light radius around the player | Rendering techniques |
| ST-08 | Raycasting first-person view of the same map | Core Doom/Wolfenstein technique |
