# Build Plan: Top-Down Demon Shooter

Work one milestone at a time. Build, run and test before moving on. Each milestone ends with something you can see on screen.

Time estimates are rough, assuming you build with an AI assistant and understand the code as you go.

| Milestone | Topic | Est. time |
|---|---|---|
| M0 | Project skeleton and window | 0.5 h |
| M1 | Tile map load and render | 1 h |
| M2 | Player movement, collision, aiming | 1.5 h |
| M3 | Shooting and bullets | 1.5 h |
| M4 | Chaser enemy, damage, death | 2 h |
| M5 | Pickups and HUD | 1 h |
| M6 | Shooter enemy and enemy projectiles | 1.5 h |
| M7 | Key, door, exit, game states | 1.5 h |
| M8 | Polish and tuning | 1 h |
| M9 | One stretch goal (optional) | 2 to 4 h |
| M10 | Testing, screenshots, report | 3 h |

Minimum viable submission: M0 to M7. If time runs short, cut in this order: M9, then M8, then the Shooter enemy in M6.

---

## M0: Project Skeleton and Window

Tasks
- [ ] Create `CMakeLists.txt` and the `src/` folder
- [ ] `Config.h` with window size, tile size, FPS
- [ ] `Game` class with `run()` and the `GameState` enum
- [ ] Menu screen with title and "Press Enter"
- [ ] Esc quits from the menu

Done when: the window opens at 1024 x 576 showing the menu, and the build has no warnings. (AC-01, AC-19)

---

## M1: Tile Map Load and Render

Tasks
- [ ] `TileMap::load` reads `levels/level1.txt`
- [ ] Validation from requirements section 8, with line and column in error text
- [ ] `TileMap::draw` renders walls, floor, door, exit in different colors
- [ ] Enter on the menu loads the level and switches to Playing
- [ ] Bad file path shows an error on the menu instead of crashing

Done when: the sample level fills the map area and a deliberately broken copy shows a readable error. (AC-02, AC-17)

---

## M2: Player Movement, Collision, Aiming

Tasks
- [ ] Player spawns at `P`
- [ ] WASD movement, normalized diagonal, delta-time based
- [ ] `collision::moveWithCollision` with separate X and Y resolution
- [ ] Aim direction from the mouse position, drawn as a line from the player

Done when: you can run around the whole level, slide along walls, and never get stuck in corners. (AC-03, AC-04, AC-05)

---

## M3: Shooting and Bullets

Tasks
- [ ] `std::vector<Bullet>` owned by `Game`
- [ ] Hold left mouse to fire at 4 shots per second
- [ ] Ammo decreases by 1 per shot, no firing at 0
- [ ] Bullets die on walls and after their lifetime
- [ ] Temporary ammo text on screen for testing

Done when: bullets fly toward the cursor, stop at walls, and ammo counts down. (AC-06, AC-07)

---

## M4: Chaser Enemy, Damage, Death

Tasks
- [ ] Spawn Chasers from `Z` tiles
- [ ] `collision::lineOfSight`
- [ ] Chaser state machine: Idle, Chase, Attack
- [ ] Contact damage with 0.8 s cooldown
- [ ] Bullets damage enemies, enemies die at 0 health, kill counter increases
- [ ] Player health, Game Over state, R to restart

Done when: Chasers ignore you out of sight, hunt you when they see you, can be killed, and can kill you. (AC-08, AC-10, AC-14, AC-16)

---

## M5: Pickups and HUD

Tasks
- [ ] Spawn Health and Ammo pickups from `H` and `A`
- [ ] Touch to collect, clamp to maximum, remove pickup
- [ ] `Hud` draws the bottom bar: health bar, ammo, kills, time

Done when: pickups work and the HUD shows live values. (AC-11)

---

## M6: Shooter Enemy and Enemy Projectiles

Tasks
- [ ] Spawn Shooters from `S`
- [ ] Idle and Engage state machine with the 180 to 260 px distance band
- [ ] Enemy projectiles reuse `Bullet` with `fromPlayer = false`
- [ ] Projectile vs player damage

Done when: Shooters hold their distance, fire visible projectiles, and can be killed. (AC-09)

---

## M7: Key, Door, Exit, Game States

Tasks
- [ ] Keycard pickup sets `hasKey` and shows the key icon
- [ ] Door blocks movement until touched with the key, then opens permanently
- [ ] Exit touch gives Level Complete with kills and time
- [ ] Pause with Esc, full freeze while paused
- [ ] All transitions from requirements section 9

Done when: you can play from the menu to the end screen and restart cleanly. (AC-12, AC-13, AC-15, AC-16)

---

## M8: Polish and Tuning

Tasks
- [ ] Hit flash on enemies and player (FR-20)
- [ ] Enemies push apart (FR-19)
- [ ] Wake enemies when hit (FR-21)
- [ ] Tune speeds, damage, and pickup placement so the level feels fair
- [ ] Optional: muzzle flash, screen shake, sounds

Done when: the level is beatable but not trivial, and the game feels responsive.

---

## M9: One Stretch Goal (Optional)

Pick exactly one from requirements section 13. Suggested for a competitive programmer: ST-02 BFS pathfinding, because it gives a strong algorithm section in the report.

---

## M10: Testing, Screenshots, Report

Tasks
- [ ] Walk through every Acceptance Criterion in requirements section 12 and tick it
- [ ] Record any failed criterion and fix or document it as a limitation
- [ ] Take screenshots: menu, gameplay, key and door, game over, level complete
- [ ] Record a 1 to 2 minute gameplay video
- [ ] Fill in REPORT_OUTLINE.md
- [ ] Re-read every source file once so you can explain it in the viva

Done when: all must-have criteria are ticked and the report is drafted.
