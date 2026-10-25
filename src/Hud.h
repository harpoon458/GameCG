#pragma once

#include <string>

namespace hud {

void drawHud(int health, int ammo, bool hasKey, int level, int kills, float elapsedTime);
void drawMenu(bool hasError, const std::string& error);
void drawPaused();
void drawGameOver();
void drawLevelComplete(int level, int kills, float elapsedTime);
void drawAllLevelsComplete(int kills, float elapsedTime);

}
