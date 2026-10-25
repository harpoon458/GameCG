# Demon Shooter

A top-down 2D shooter inspired by classic Doom, written in C++17 with [raylib](https://www.raylib.com/). Fight through ten demon-infested levels, collect weapons supplies, find the keycard, unlock the exit door, and escape.

Everything is drawn with simple colored shapes, so there are no asset files to install.

---

## Table of Contents

- [About the Game](#about-the-game)
- [Features](#features)
- [Requirements](#requirements)
- [Installing Dependencies](#installing-dependencies)
  - [Arch Linux](#arch-linux)
  - [Debian / Ubuntu / Linux Mint](#debian--ubuntu--linux-mint)
  - [Fedora](#fedora)
  - [macOS](#macos)
  - [Windows](#windows)
- [Build and Run](#build-and-run)
- [Controls](#controls)
- [Gameplay Rules](#gameplay-rules)
- [Enemies and Pickups](#enemies-and-pickups)
- [Levels](#levels)
- [Level File Format](#level-file-format)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)

---

## About the Game

You are a lone space marine trapped in a demon-filled complex. Each level is a 32 x 16 tile dungeon containing roaming enemies, health packs, ammo packs, and a locked door guarding the exit. Find the gold keycard, open the door, and reach the exit tile to advance.

There are 10 hand-checked levels with a rising difficulty curve: more enemies, tighter rooms, fewer supplies, and more cover for the demons to use. Health and ammo reset when a new level starts, while your total kills and time carry across the campaign.

The game is a university assignment project focused on clean C++17, data-driven levels, and a complete playable loop rather than large amounts of content.

## Features

- 10 levels loaded from plain-text map files
- Two enemy types with different AI (chasers and ranged shooters)
- Pistol combat with rate-limited firing, ammo use, and hit feedback
- Health and ammo pickups that clamp to their maximum values
- Keycard and locked-door progression, plus an exit tile
- HUD with health, ammo, key status, kills, and elapsed time
- Menu, pause, game over, level complete, and campaign complete screens
- Wall sliding collision, line-of-sight checks, and enemy separation
- Frame-rate independent movement clamped to prevent tunneling
- All tunable values collected in `src/Config.h`

## Requirements

| Tool | Version |
|---|---|
| C++ compiler | C++17 capable (GCC, Clang, or MSVC) |
| CMake | 3.16 or newer |
| raylib | 5.0 or newer |
| Git | only needed if raylib is downloaded automatically |

CMake first looks for an installed raylib. If it cannot find one (or finds a version older than 5.0), it downloads and builds raylib 5.5 automatically using Git and an internet connection.

## Installing Dependencies

### Arch Linux

```bash
sudo pacman -S --needed base-devel cmake raylib git
```

### Debian / Ubuntu / Linux Mint

```bash
sudo apt update
sudo apt install -y build-essential cmake git \
  libasound2-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev \
  libxinerama-dev libgl1-mesa-dev libwayland-dev libxkbcommon-dev \
  wayland-protocols
```

You can also install the system raylib package. If it is older than 5.0, CMake will fetch a newer copy:

```bash
sudo apt install -y libraylib-dev
```

### Fedora

```bash
sudo dnf install -y gcc-c++ make cmake git \
  alsa-lib-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel \
  libXinerama-devel mesa-libGL-devel wayland-devel libxkbcommon-devel \
  wayland-protocols-devel
```

Optional system raylib:

```bash
sudo dnf install -y raylib-devel
```

### macOS

Install the Xcode command line tools, then CMake and Git via Homebrew:

```bash
xcode-select --install
brew install cmake git
```

Optional system raylib:

```bash
brew install raylib
```

If raylib is not installed, the CMake build downloads it automatically.

### Windows

The recommended setup is [MSYS2](https://www.msys2.org/). Open the **UCRT64** shell and run:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-raylib \
  git
```

Then build from the project folder using the commands below, from the same UCRT64 shell.

Alternative: install Visual Studio with the **Desktop development with C++** workload, plus CMake and Git. Let CMake fetch raylib, then configure with the Visual Studio generator. When using a multi-config generator, run the executable from `build\Debug\` (or `build\Release\`).

## Build and Run

From the project root:

```bash
cmake -S . -B build
cmake --build build -j
```

Run the game **from the build directory**, because level files are loaded with the relative path `levels/levelN.txt`:

```bash
cd build
./demon_shooter
```

On Windows (MSYS2 / PowerShell):

```powershell
cd build
.\demon_shooter.exe
```

CMake automatically copies the `levels/` folder next to the executable after every build, so a fresh clone just needs the commands above.

To rebuild after changing source files:

```bash
cmake --build build -j
```

## Controls

| Input | Action |
|---|---|
| `W` `A` `S` `D` | Move (8 directions) |
| Mouse | Aim |
| Left mouse button (hold) | Fire the pistol |
| `Enter` | Start game from the menu; continue after level complete |
| `Esc` | Pause / resume; returns to the menu from end screens; quits from the menu |
| `R` | Restart the current level (from pause, game over, or level complete) |

## Gameplay Rules

- **Win a level:** touch the exit tile (`E`).
- **Lose:** your health reaches 0.
- **Supply pickups** apply instantly on touch and disappear. Health is capped at 100 and ammo at 99.
- **Keycard:** pick up the gold keycard, then walk into the locked door to open it permanently. The key is not consumed.
- **Ammo:** each shot costs 1 ammo. You cannot fire at 0 ammo.
- **Enemies:** killing enemies is optional, but kills are tracked for the end-of-level and campaign scores.
- **Between levels:** health, ammo, and the key reset. Kills and elapsed time carry over.
- **After level 10:** a campaign complete screen shows your total kills and time.

## Enemies and Pickups

| Entity | Behavior |
|---|---|
| Chaser (red) | Idles until it sees you, then pursues and deals contact damage every 0.8 s |
| Shooter (orange) | Keeps its distance and fires slow purple projectiles |
| Health pack (green) | Restores 25 health, up to 100 |
| Ammo pack (blue) | Restores 12 ammo, up to 99 |
| Keycard (gold) | Opens the locked door |
| Door | Solid until touched while holding the keycard |
| Exit | Ends the level when touched |

## Levels

The game ships with ten levels in `levels/level1.txt` through `levels/level10.txt`, played in order. Level 1 is hand-made; levels 2 to 10 were generated with fixed seeds and validated automatically. See `levels/LEVELS.md` for a full guide, maps, and the difficulty ramp.

Levels 2 to 10 can be regenerated or retuned with:

```bash
python3 levels/generate_levels.py levels
```

Level 1 is never modified by the generator.

## Level File Format

Each level is a plain-text grid, one character per 32 x 32 pixel tile, 32 columns by 16 rows.

| Char | Meaning |
|---|---|
| `#` | Wall |
| `.` | Floor |
| `P` | Player start |
| `Z` | Chaser spawn |
| `S` | Shooter spawn |
| `H` | Health pack |
| `A` | Ammo pack |
| `K` | Keycard |
| `D` | Locked door |
| `E` | Exit |

A valid level must be fully enclosed by walls, have exactly one `P` and one `E`, and (if a door exists) a reachable keycard. Invalid files produce an on-screen error instead of crashing.

## Project Structure

```text
.
├── CMakeLists.txt          # Build file; finds or downloads raylib
├── src/
│   ├── main.cpp            # Entry point
│   ├── Game.h/.cpp         # Game loop, states, level loading
│   ├── Config.h            # All tunable constants
│   ├── Player.h/.cpp       # Movement, aiming, shooting
│   ├── Enemy.h/.cpp        # Chaser and shooter AI
│   ├── Bullet.h/.cpp       # Player and enemy projectiles
│   ├── Pickup.h/.cpp       # Health, ammo, and keycard pickups
│   ├── TileMap.h/.cpp      # Level parsing and rendering
│   ├── Collision.h/.cpp    # Circle, tile, and line-of-sight helpers
│   └── Hud.h/.cpp          # HUD and overlay screens
├── levels/
│   ├── level1.txt ... level10.txt
│   ├── LEVELS.md           # Level guide and format notes
│   └── generate_levels.py  # Level generator for levels 2 to 10
├── requirements.md         # Full feature and gameplay specification
├── ARCHITECTURE.md         # Code structure and update order
├── BUILD_PLAN.md           # Development milestones
└── REPORT_OUTLINE.md       # Assignment report skeleton
```

## Troubleshooting

**`Could not load level levels/level1.txt` (or an on-screen level error)**

The game was started from the wrong folder. Run it from the `build` directory so the `levels/` folder is found:

```bash
cd build
./demon_shooter
```

**CMake cannot find raylib and the download fails**

Check that Git is installed and you have an internet connection. On Linux, make sure the X11/Wayland and ALSA development packages listed above are installed, since raylib must build against them.

**Wayland or X11 errors on Linux**

The build enables Wayland support when downloading raylib. If your system only has X11 development files, install your distribution's Wayland development packages, or install a system raylib package instead.

**Black window or missing OpenGL**

Update your graphics drivers. The game needs a working OpenGL 3.3 context provided by raylib.
