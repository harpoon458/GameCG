#include "Game.h"

#include "Collision.h"
#include "Config.h"
#include "Hud.h"
#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace {

std::string levelPath(int level)
{
    return "levels/level" + std::to_string(level) + ".txt";
}

}

bool Game::loadLevel(int levelNumber, bool resetCampaign)
{
    bullets.clear();
    enemies.clear();
    pickups.clear();
    levelKills = 0;
    levelTime = 0.0f;
    levelError.clear();
    currentLevel = levelNumber;

    if (resetCampaign) {
        totalKills = 0;
        totalTime = 0.0f;
    }

    std::string error;
    if (!map.load(levelPath(levelNumber), error)) {
        levelError = error;
        return false;
    }

    player.position = map.playerStart();
    player.aimDirection = Vector2{1.0f, 0.0f};
    player.health = cfg::playerMaxHealth;
    player.ammo = cfg::playerStartingAmmo;
    player.hasKey = false;
    player.fireCooldown = 0.0f;
    player.hitFlashTimer = 0.0f;

    for (const SpawnPoint& spawn : map.spawns()) {
        const Vector2 position{static_cast<float>(spawn.col * cfg::tileSize) + cfg::tileSize / 2.0f,
                               static_cast<float>(spawn.row * cfg::tileSize) + cfg::tileSize / 2.0f};
        switch (spawn.kind) {
        case 'Z':
            enemies.push_back(makeEnemy(EnemyType::Chaser, position));
            break;
        case 'S':
            enemies.push_back(makeEnemy(EnemyType::Shooter, position));
            break;
        case 'H':
            pickups.push_back(makePickup(PickupType::Health, position));
            break;
        case 'A':
            pickups.push_back(makePickup(PickupType::Ammo, position));
            break;
        case 'K':
            pickups.push_back(makePickup(PickupType::Key, position));
            break;
        default:
            break;
        }
    }

    return true;
}

void Game::startNewGame()
{
    if (loadLevel(1, true)) {
        state = GameState::Playing;
    } else {
        state = GameState::Menu;
    }
}

void Game::restartLevel()
{
    if (loadLevel(currentLevel, false)) {
        state = GameState::Playing;
    } else {
        state = GameState::Menu;
    }
}

void Game::advanceLevel()
{
    totalKills += levelKills;
    totalTime += levelTime;

    if (currentLevel >= cfg::levelCount) {
        state = GameState::AllLevelsComplete;
        return;
    }

    if (loadLevel(currentLevel + 1, false)) {
        state = GameState::Playing;
    } else {
        state = GameState::Menu;
    }
}

void Game::handleInput()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        switch (state) {
        case GameState::Menu:
            quitRequested = true;
            break;
        case GameState::Playing:
            state = GameState::Paused;
            break;
        case GameState::Paused:
            state = GameState::Playing;
            break;
        case GameState::GameOver:
        case GameState::LevelComplete:
        case GameState::AllLevelsComplete:
            state = GameState::Menu;
            break;
        }
    }

    if (state == GameState::Menu && IsKeyPressed(KEY_ENTER)) {
        if (levelError.empty()) {
            startNewGame();
        } else {
            restartLevel();
        }
    }

    if (state == GameState::LevelComplete && IsKeyPressed(KEY_ENTER)) {
        advanceLevel();
    }

    if (state == GameState::AllLevelsComplete && IsKeyPressed(KEY_ENTER)) {
        startNewGame();
    }

    if ((state == GameState::Paused || state == GameState::GameOver || state == GameState::LevelComplete) &&
        IsKeyPressed(KEY_R)) {
        restartLevel();
    }

    if (state == GameState::AllLevelsComplete && IsKeyPressed(KEY_R)) {
        startNewGame();
    }
}

void Game::updateEnemies(float dt)
{
    for (Enemy& enemy : enemies) {
        if (enemy.alive) {
            updateEnemy(enemy, player, map, bullets, dt);
        }
    }
    separateEnemies(enemies, map);
}

void Game::handleCombat()
{
    for (Bullet& bullet : bullets) {
        if (!bullet.alive) {
            continue;
        }

        if (bullet.fromPlayer) {
            for (Enemy& enemy : enemies) {
                if (!enemy.alive) {
                    continue;
                }
                if (collision::circlesOverlap(bullet.position, bullet.radius, enemy.position, enemy.radius)) {
                    bullet.alive = false;
                    if (damageEnemy(enemy, bullet.damage)) {
                        ++levelKills;
                    }
                    break;
                }
            }
        } else if (player.health > 0 &&
                   collision::circlesOverlap(bullet.position, bullet.radius, player.position, cfg::playerRadius)) {
            bullet.alive = false;
            player.health -= bullet.damage;
            player.hitFlashTimer = cfg::playerHitFlashDuration;
        }
    }

    for (Enemy& enemy : enemies) {
        if (!enemy.alive || enemy.type != EnemyType::Chaser || player.health <= 0) {
            continue;
        }
        if (collision::circlesOverlap(enemy.position, enemy.radius, player.position, cfg::playerRadius) &&
            enemy.attackCooldown <= 0.0f) {
            player.health -= cfg::chaserContactDamage;
            player.hitFlashTimer = cfg::playerHitFlashDuration;
            enemy.attackCooldown = cfg::chaserContactInterval;
        }
    }
}

void Game::handlePickups()
{
    for (Pickup& pickup : pickups) {
        if (!pickup.collected && pickupTouchesPlayer(pickup, player.position, cfg::playerRadius)) {
            if (pickup.type == PickupType::Health) {
                player.health = std::min(cfg::playerMaxHealth, player.health + cfg::healthPackAmount);
            } else if (pickup.type == PickupType::Ammo) {
                player.ammo = std::min(cfg::playerMaxAmmo, player.ammo + cfg::ammoPackAmount);
            } else {
                player.hasKey = true;
            }
            pickup.collected = true;
        }
    }
}

bool Game::handleDoorAndExit()
{
    bool reachedExit = false;

    for (int row = 0; row < map.rows(); ++row) {
        for (int col = 0; col < map.cols(); ++col) {
            const Tile tile = map.at(col, row);
            if (tile == Tile::Door) {
                if (player.hasKey &&
                    collision::circleTouchesTile(player.position, cfg::playerRadius + cfg::doorTouchPadding, col, row)) {
                    map.openDoor(col, row);
                }
            } else if (tile == Tile::Exit) {
                if (collision::circleTouchesTile(player.position, cfg::playerRadius, col, row)) {
                    reachedExit = true;
                }
            }
        }
    }

    return reachedExit;
}

void Game::cleanup()
{
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) { return !bullet.alive; }),
                  bullets.end());
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) { return !enemy.alive; }),
                  enemies.end());
    pickups.erase(std::remove_if(pickups.begin(), pickups.end(), [](const Pickup& pickup) { return pickup.collected; }),
                  pickups.end());
}

void Game::update(float dt)
{
    levelTime += dt;

    updatePlayer(player, map, bullets, dt);
    updateEnemies(dt);

    for (Bullet& bullet : bullets) {
        updateBullet(bullet, map, dt);
    }

    handleCombat();
    handlePickups();
    const bool reachedExit = handleDoorAndExit();
    cleanup();

    player.health = std::max(0, player.health);

    if (player.health <= 0) {
        state = GameState::GameOver;
    } else if (reachedExit) {
        if (currentLevel >= cfg::levelCount) {
            totalKills += levelKills;
            totalTime += levelTime;
            levelKills = 0;
            levelTime = 0.0f;
            state = GameState::AllLevelsComplete;
        } else {
            state = GameState::LevelComplete;
        }
    }
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(Color{10, 10, 13, 255});

    if (levelError.empty()) {
        map.draw();

        for (const Pickup& pickup : pickups) {
            drawPickup(pickup);
        }
        for (const Enemy& enemy : enemies) {
            drawEnemy(enemy);
        }
        for (const Bullet& bullet : bullets) {
            drawBullet(bullet);
        }

        drawPlayer(player);
    }

    if (state != GameState::Menu) {
        hud::drawHud(player.health, player.ammo, player.hasKey, currentLevel,
                     totalKills + levelKills, totalTime + levelTime);
    }

    switch (state) {
    case GameState::Menu:
        hud::drawMenu(!levelError.empty(), levelError);
        break;
    case GameState::Playing:
        break;
    case GameState::Paused:
        hud::drawPaused();
        break;
    case GameState::GameOver:
        hud::drawGameOver();
        break;
    case GameState::LevelComplete:
        hud::drawLevelComplete(currentLevel, totalKills + levelKills, totalTime + levelTime);
        break;
    case GameState::AllLevelsComplete:
        hud::drawAllLevelsComplete(totalKills, totalTime);
        break;
    }

    EndDrawing();
}

void Game::run()
{
    InitWindow(cfg::screenWidth, cfg::screenHeight, cfg::windowTitle);
    SetExitKey(KEY_NULL);
    SetTargetFPS(cfg::targetFps);

    loadLevel(1, false);

    while (!WindowShouldClose() && !quitRequested) {
        const float dt = std::min(GetFrameTime(), cfg::maxFrameTime);

        handleInput();
        if (state == GameState::Playing) {
            update(dt);
        }
        draw();
    }

    CloseWindow();
}
