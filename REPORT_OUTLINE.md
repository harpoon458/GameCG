# Report Outline: Top-Down Demon Shooter

Use this as the skeleton of the assignment report. Fill each section from your actual implementation. Lengths are suggestions for a 10 to 15 page report.

## 1. Title Page and Abstract (0.5 page)
- Project title, name, course, date
- Abstract in 100 to 150 words: what the game is, the tools used, the main techniques, and the result

## 2. Introduction (1 page)
- Background: Doom and the top-down shooter genre
- Problem statement: build a complete 2D shooter with enemies, items, and level progression
- Motivation for choosing this project

## 3. Objectives and Scope (0.5 page)
- List objectives (playable level, two enemy types, pickups, key and door, HUD, states)
- In scope and out of scope (copy from requirements.md sections 2.1 and 2.2)

## 4. Tools and Technologies (0.5 page)
- C++17, raylib, CMake, Linux, editor and compiler used
- Why raylib: simple immediate-mode drawing, input and timing; no engine overhead
- Why a tile map: easy collision, easy level design from plain text

## 5. Requirements Analysis (1 to 1.5 pages)
- Functional requirements table (summarize FR-01 to FR-18)
- Non-functional requirements (performance, code quality)
- Controls table
- Game rules

## 6. System Design (2 to 3 pages)
- Architecture block diagram (modules from ARCHITECTURE.md section 2)
- Class and struct overview
- Game state diagram (requirements section 9)
- Game loop flowchart (ARCHITECTURE.md section 5)
- Level file format and example excerpt

## 7. Implementation (3 to 4 pages)
Write one subsection per topic. Include a short code excerpt only where it helps.

| Subsection | What to explain |
|---|---|
| 7.1 Game loop and delta time | Fixed order of update steps, frame-rate independence, dt clamp |
| 7.2 Tile map and level loading | Parsing, validation, spawn list |
| 7.3 Player movement and aiming | Input vector, normalization, mouse direction |
| 7.4 Collision detection | Circle vs tile, axis-separated resolution, sliding |
| 7.5 Shooting and projectiles | Fire rate, ammo, bullet lifetime, tunneling avoidance |
| 7.6 Enemy AI | Chaser and Shooter state machines, line of sight |
| 7.7 Pickups, key and door | Touch tests, clamping, permanent door opening |
| 7.8 HUD and game states | Overlay rendering, state transitions |
| 7.9 Stretch feature (if built) | For BFS: algorithm, complexity, integration |

## 8. Algorithms and Complexity (0.5 to 1 page)
- Collision check per entity: constant, since only nearby tiles are tested
- Line of sight: O(distance / step) samples per check
- Entity-vs-entity tests: O(bullets x enemies) per frame
- BFS pathfinding if implemented: O(cols x rows) per search

## 9. Testing (1 to 1.5 pages)
- Table of the 20 acceptance criteria with pass or fail and notes
- Extra tests: corner collision, firing with 0 ammo, restart after each end state, broken level file
- Bugs found and how they were fixed (two or three real examples)

Test table template:

| ID | Test | Expected | Result | Notes |
|---|---|---|---|---|
| AC-03 | Hold W and D together | Same speed as W alone | | |
| AC-06 | Hold left click for 5 s | About 20 shots, ammo drops by 20 | | |

## 10. Results (1 page)
- Screenshots: menu, gameplay with enemies, key pickup, open door, game over, level complete
- Link or description of the gameplay video
- Measured frame rate

## 11. Limitations and Future Work (0.5 page)
- Chasers can get stuck behind corners without pathfinding
- Single level, single weapon
- Future work: shotgun, boss, multiple levels, sound, raycast first-person mode

## 12. Conclusion (0.5 page)
- What was achieved and what you learned

## 13. References
- raylib documentation and cheatsheet
- Reference material on game loops, tile-based collision, and BFS (textbook or tutorial you actually used)
- Doom (id Software, 1993) as inspiration

## Appendix: Viva Questions with Model Answers

1. What is a game loop?
   A loop that repeatedly reads input, updates the game state, and draws a frame until the player quits. Here it runs at 60 FPS inside `Game::run`.

2. Why use delta time?
   So movement and timers depend on elapsed real time, not on frame rate. A 200 px/s player moves the same distance on a slow or fast machine. The value is clamped to 1/30 s to avoid huge jumps.

3. How is collision with walls handled?
   Entities are circles. Movement is applied on X, collisions resolved against nearby solid tiles, then applied on Y and resolved again. Separating the axes makes the player slide along walls.

4. Why normalize the diagonal movement vector?
   Without it, moving diagonally gives speed of about 1.41 times the straight speed.

5. How does the player aim at the mouse?
   Subtract the player position from the mouse position and normalize. That unit vector is the bullet direction and the facing indicator.

6. Why can fast bullets be a problem and how is it avoided?
   A bullet could jump over a wall between frames (tunneling). Its per-frame travel is kept below one tile by limiting speed and clamping dt.

7. How do enemies know when to chase?
   They check distance against sight range and run a line-of-sight test that samples points along the line and fails if any point is inside a wall.

8. What is a state machine and where is it used?
   A model where an entity is in exactly one state at a time and transitions on conditions. Enemies use Idle, Chase, Attack or Idle, Engage. The game uses Menu, Playing, Paused, GameOver, LevelComplete.

9. Why load levels from text files?
   Levels become data, so designers can change them without recompiling, and new levels need no code changes.

10. Why plain structs and free functions instead of class inheritance?
    The game has few entity types, so this stays simpler to read and explain. Inheritance would add indirection without a real benefit at this size.

11. How does BFS pathfinding work and what is its complexity? (if implemented)
    It explores tiles in layers outward from the start using a queue, recording each tile's parent, until the goal is reached. Then it walks parents backward to build the path. Time and space are O(cols x rows).

12. What would you improve with more time?
    Pathfinding for all enemies, more weapons and levels, sound, and a raycast first-person view.
