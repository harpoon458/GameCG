# Levels: Top-Down Demon Shooter

Ten levels in the format from `requirements.md` section 8. Level 1 is the hand-made sample. Levels 2 to 10 were produced by `tools/generate_levels.py` and each one was checked automatically.

The game reads the level files as plain text, so they are `.txt` files. This document is the guide to them.

## 1. Legend

| Char | Meaning |
|---|---|
| `#` | Wall |
| `.` | Floor |
| `P` | Player start |
| `Z` | Chaser |
| `S` | Shooter |
| `H` | Health pack |
| `A` | Ammo pack |
| `K` | Keycard |
| `D` | Locked door |
| `E` | Exit |

## 2. Difficulty Ramp

Shots per enemy is the ammo you can collect (30 starting plus 12 per ammo pack) divided by the number of enemies. Killing any enemy takes at least 2 bullets, so values near 2 leave almost no room to miss.

| Level | Name | Chasers | Shooters | Total | Health packs | Ammo packs | Shots per enemy | Enemies per 100 floor tiles | Route (steps) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | First Blood | 4 | 3 | 7 | 2 | 2 | 7.7 | 2 | 58 |
| 2 | Warm-Up | 5 | 3 | 8 | 3 | 2 | 6.8 | 6 | 59 |
| 3 | Cover | 6 | 4 | 10 | 3 | 3 | 6.6 | 7 | 52 |
| 4 | Crossfire | 7 | 5 | 12 | 3 | 4 | 6.5 | 8 | 50 |
| 5 | Loops | 8 | 6 | 14 | 3 | 4 | 5.6 | 9 | 57 |
| 6 | Pressure | 9 | 7 | 16 | 3 | 5 | 5.6 | 9 | 64 |
| 7 | Thin Supplies | 10 | 8 | 18 | 4 | 5 | 5.0 | 11 | 54 |
| 8 | Tight Quarters | 11 | 9 | 20 | 4 | 5 | 4.5 | 12 | 58 |
| 9 | Gauntlet | 12 | 10 | 22 | 4 | 4 | 3.5 | 12 | 65 |
| 10 | Final Descent | 13 | 12 | 25 | 4 | 4 | 3.1 | 13 | 64 |

Route is the walking distance from the start to the keycard plus the distance from the keycard to the exit, in tiles.

### What gets harder

| Factor | Level 1 | Level 10 |
|---|---|---|
| Enemies | 7 | 25 |
| Shooter share of enemies | 43% | 48% |
| Shots available per enemy | 7.7 | 3.1 |
| Enemies per 100 floor tiles | 2 | 13 |
| Cover pillars | none | up to 16 |
| Extra loops that let enemies flank | none | 2 |

Enemy count rises on every level and ammo per enemy falls overall. Route length is not strictly increasing, because each layout is different. Every generated level has at least `40 + 2 x level` steps between start, key and exit.

## 3. Notes on Balance

- None of these levels has been playtested in your game. The numbers come from the stats in `requirements.md`, so expect to tune enemy counts or pickups after you try a few levels.
- Levels 2 to 10 are more compact than level 1. They have 135 to 192 floor tiles against 344, so enemies are packed more tightly.
- Pillars and corners make a difference once enemies walk straight at the player. Without pathfinding (stretch goal ST-02), chasers can get stuck behind pillars on the higher levels.
- Each generated level has one door, and the exit is behind it. The keycard is always on the start side of the door and as far from the start as possible. The first health pack and first ammo pack are also on the start side.
- No enemy spawns within 8 steps of the player start.

## 4. The Levels

### Level 1: First Blood

Hand-made, open rooms. 7 enemies (4 chasers, 3 shooters), 2 health packs, 2 ammo packs.
The keycard is 16 steps from the start, and the exit is 42 steps beyond the keycard. File: `levels/level1.txt`

```text
################################
#P.....#.......#...............#
#......#...Z...#.......S.......#
#..H...........................#
#......#.......#...............#
####.###########.......A.......#
#..............................#
#..Z...........#######.#########
#..............#.....#.#.......#
#......A.......#..H..#.#...Z...#
#..............#.....#.#.......#
####.###########..S..#.#.......#
#.....#..........#####.###.#####
#..K..#.....Z..........#.....E.#
#.....#................D...S...#
################################
```

### Level 2: Warm-Up

Up to 5 rooms, no pillars. 8 enemies (5 chasers, 3 shooters), 3 health packs, 2 ammo packs.
The keycard is 39 steps from the start, and the exit is 20 steps beyond the keycard. File: `levels/level2.txt`

```text
################################
##########################..A.E#
####P...##....############S....#
####...H##Z.........######.....#
####....##...Z#####.######.....#
######.#####.###.....S.###...Z.#
######.#####.###.........D...###
######.#####.###K......#########
######.#####.###################
######.#A...Z...################
######.#..H.....################
######.........S################
########H.Z.....################
########........################
################################
################################
```

### Level 3: Cover

Up to 6 rooms, up to 2 pillars. 10 enemies (6 chasers, 4 shooters), 3 health packs, 3 ammo packs.
The keycard is 39 steps from the start, and the exit is 13 steps beyond the keycard. File: `levels/level3.txt`

```text
################################
##...Z....######################
##Z#......######################
##.......S....#############.A.E#
##....Z...#..Z.############....#
######.####........####...D..H.#
######.####....###.####.###....#
######.###########.##.S..K#..Z.#
######.#########.A..#.....######
######.#########......H...######
#P.....#########S..Z#.#...######
#....A.##############..S.H######
#......#########################
#.....##########################
################################
################################
```

### Level 4: Crossfire

Up to 6 rooms, up to 4 pillars. 12 enemies (7 chasers, 5 shooters), 3 health packs, 4 ammo packs.
The keycard is 40 steps from the start, and the exit is 10 steps beyond the keycard. File: `levels/level4.txt`

```text
################################
#######....#####################
#######...Z#####################
#######H...#####################
#########.#####A..A...##########
#########.#####....#..##########
#########.#####..Z..Z....S..S..#
####P...H.#####.......###......#
####..#...######.########.A....#
####......######.########.....K#
#########.##S..Z..S.#######D####
#########.##........####A......#
#########.......#...####.....#.#
############Z..Z..S.####E..H..Z#
################################
################################
```

### Level 5: Loops

Up to 7 rooms, up to 6 pillars, 1 extra loop. 14 enemies (8 chasers, 6 shooters), 3 health packs, 4 ammo packs.
The keycard is 33 steps from the start, and the exit is 24 steps beyond the keycard. File: `levels/level5.txt`

```text
################################
################################
###.S....#######################
###.#...Z###H....####.....A#####
###..H.#......Z.......S......###
###....#.###....Z###.....H.#.###
###A.Z...####.######.#######D###
######.####....##..S....##.....#
######.####Z.Z.##.#..S..##....S#
######.####.#..##K......##.....#
#P..A..####Z...###########E.Z.S#
#.#....####....#################
#...A.##########################
################################
################################
################################
```

### Level 6: Pressure

Up to 7 rooms, up to 8 pillars, 1 extra loop. 16 enemies (9 chasers, 7 shooters), 3 health packs, 5 ammo packs.
The keycard is 46 steps from the start, and the exit is 18 steps beyond the keycard. File: `levels/level6.txt`

```text
################################
#########################...A.E#
#########################......#
#############S....#######A....A#
#####.S...Z##....Z#########D####
#####......##..A..#########.####
#####.A#S....Z##..#...S.H##.####
#####..#..H##...S.#.Z....##.####
#####......####.###...S.....####
#######.#######.###.S...Z##.####
#######.#######.######.####.####
####P......####....###.#......##
####..#H...####..Z.....#..##..##
####..#....####....#####K..Z.Z##
####......Z#####################
################################
```

### Level 7: Thin Supplies

Up to 8 rooms, up to 10 pillars, 1 extra loop. 18 enemies (10 chasers, 8 shooters), 4 health packs, 5 ammo packs.
The keycard is 36 steps from the start, and the exit is 18 steps beyond the keycard. File: `levels/level7.txt`

```text
################################
###########.A.H.################
#######.........###....#########
#######.###H..A.....S.S#########
#######.#####.#.###....##S..Z.E#
#####P....###.#.#####.##..#....#
#####.##..#Z....#####.##...A..S#
#####..#..#.#Z..#####.##D#######
#####.....#.....#####......#####
#####..H.H#.#.S.#####Z#Z.Z.#####
###########.S...#####......#####
#############.#.#####.##.#######
#############.Z..S##A.....Z#####
#############..##.##.#.Z...#####
#############.S..Z##A.....K#####
################################
```

### Level 8: Tight Quarters

Smaller rooms, up to 12 pillars, 2 extra loops. 20 enemies (11 chasers, 9 shooters), 4 health packs, 5 ammo packs.
The keycard is 42 steps from the start, and the exit is 16 steps beyond the keycard. File: `levels/level8.txt`

```text
################################
######Z.S.Z.####.K.Z.S##########
#####....##.####.##......#.H..##
#####...A..A####...S.Z##.D.#.E##
#####.###.########..####.#....##
#####.###.#####H.....###.#######
#####.###.#####..Z.#Z###.#######
#####.###.#####S.....###.#######
#####.###.########..####.#######
#####.###.S...####..##..Z.######
##P....A#..#S.......##S#..######
##..A...#.Z...#.Z..S......######
##....#.#######...##.#..A.######
##.H...S#######...#..###########
###############.Z.Z.H###########
################################
```

### Level 9: Gauntlet

Smaller rooms, up to 14 pillars, 2 extra loops. 22 enemies (12 chasers, 10 shooters), 4 health packs, 4 ammo packs.
The keycard is 37 steps from the start, and the exit is 28 steps beyond the keycard. File: `levels/level9.txt`

```text
################################
##########..Z.A.####....K#E.Z.S#
##########Z..##.####.....#.....#
###P....##.#.#..#......#.#S....#
###.##.A##.S..Z.#.##S..Z.#.....#
###..A..#####.###.####.#####D###
#######.#####.###.####.#####.###
#######.#####.###.####.#####.###
#####H....###...Z...##.#####.###
#####..#S.###..#.#Z.##.##..S...#
#####.Z#......S.A........Z.#.H.#
#####...S.###.##.#####.##..Z...#
#############......#H.H.########
#############.S.Z#...#..########
#############.....S#.Z..########
################################
```

### Level 10: Final Descent

Smaller rooms, up to 16 pillars, 2 extra loops. 25 enemies (13 chasers, 12 shooters), 4 health packs, 4 ammo packs.
The keycard is 43 steps from the start, and the exit is 21 steps beyond the keycard. File: `levels/level10.txt`

```text
################################
################################
#####S..Z..#...S..###K....S#####
#####.#...S#...##S###.#.Z..#####
####.H.Z#..#..S#..###.S#....####
####......Z#Z.....###....S..####
####.###.######.########.##.####
####A...S######.########.##.####
####..Z#.######.###H.Z.#.##.####
####A...Z######.###.##...##.####
####.#.#.##S....###.Z.S####.####
#P.....#....#...#####.#####D####
#..#.#H#.##A.Z..#.....##.....S##
#H.##..#..........#Z..##...Z..##
#.....Z##########....###E....A##
################################
```

## 5. Using the Levels

The game loads `level1.txt` to `level10.txt` from the `levels/` folder in order.

- On Level Complete, press Enter to load the next level. Health and ammo reset to their starting values; kills and time carry over.
- After level 10, an All Levels Complete screen shows the total kills and time.
- R replays the current level, and Esc returns to the menu.

## 6. Regenerating or Changing Levels 2 to 10

```text
python3 tools/generate_levels.py levels
```

This overwrites `level2.txt` to `level10.txt` with the same maps every time, because the random seeds are fixed. Level 1 is never touched. To change difficulty, edit the tables at the top of the script (`PLAN`, `CHASERS`, `SHOOTERS`, `HEALTH`) and run it again. The script re-checks each level before writing it: correct size, fully enclosed, one start, one exit, one key, one door, the key reachable without the door, and the exit reachable only through it.
