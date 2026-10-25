#pragma once

namespace cfg {

inline constexpr int screenWidth = 1024;
inline constexpr int screenHeight = 576;
inline constexpr int targetFps = 60;
inline constexpr const char* windowTitle = "Demon Shooter";
inline constexpr int levelCount = 10;

inline constexpr int tileSize = 32;
inline constexpr int mapCols = 32;
inline constexpr int mapRows = 16;
inline constexpr int mapWidth = mapCols * tileSize;
inline constexpr int mapHeight = mapRows * tileSize;
inline constexpr int hudHeight = 64;
inline constexpr float maxFrameTime = 1.0f / 30.0f;

inline constexpr float playerRadius = 12.0f;
inline constexpr int playerMaxHealth = 100;
inline constexpr int playerMaxAmmo = 99;
inline constexpr int playerStartingAmmo = 30;
inline constexpr float playerSpeed = 200.0f;
inline constexpr float playerFireInterval = 0.25f;
inline constexpr float playerHitFlashDuration = 0.15f;
inline constexpr float muzzleFlashDuration = 0.06f;
inline constexpr float aimIndicatorLength = 34.0f;
inline constexpr float aimIndicatorThickness = 2.0f;

inline constexpr float playerBulletSpeed = 600.0f;
inline constexpr float playerBulletRadius = 3.0f;
inline constexpr float playerBulletLifetime = 1.2f;
inline constexpr int playerBulletDamage = 25;
inline constexpr float muzzleOffset = 5.0f;

inline constexpr float chaserRadius = 12.0f;
inline constexpr int chaserHealth = 50;
inline constexpr float chaserSpeed = 110.0f;
inline constexpr float chaserSightRange = 280.0f;
inline constexpr int chaserContactDamage = 10;
inline constexpr float chaserContactInterval = 0.8f;
inline constexpr float chaserTouchMargin = 3.0f;

inline constexpr float shooterRadius = 12.0f;
inline constexpr int shooterHealth = 40;
inline constexpr float shooterSpeed = 70.0f;
inline constexpr float shooterSightRange = 400.0f;
inline constexpr float shooterPreferredMin = 180.0f;
inline constexpr float shooterPreferredMax = 260.0f;
inline constexpr float shooterFireInterval = 1.5f;
inline constexpr float enemyBulletSpeed = 260.0f;
inline constexpr float enemyBulletRadius = 4.0f;
inline constexpr float enemyBulletLifetime = 3.0f;
inline constexpr int enemyBulletDamage = 8;
inline constexpr float enemyLostSightDelay = 2.0f;
inline constexpr float enemyHitFlashDuration = 0.12f;
inline constexpr float enemySeparationFactor = 0.5f;

inline constexpr float pickupRadius = 10.0f;
inline constexpr float pickupDrawRadius = 8.0f;
inline constexpr int healthPackAmount = 25;
inline constexpr int ammoPackAmount = 12;

inline constexpr float doorTouchPadding = 3.0f;
inline constexpr float lineOfSightStep = static_cast<float>(tileSize) / 4.0f;

inline constexpr float overlayDim = 0.68f;
inline constexpr int titleFontSize = 54;
inline constexpr int overlayFontSize = 46;
inline constexpr int menuFontSize = 22;
inline constexpr int hudLabelFontSize = 15;
inline constexpr int hudValueFontSize = 26;
inline constexpr float hudPadding = 16.0f;
inline constexpr float healthBarWidth = 220.0f;
inline constexpr float healthBarHeight = 22.0f;

}
