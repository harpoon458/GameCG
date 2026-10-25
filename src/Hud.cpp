#include "Hud.h"

#include "Config.h"
#include "raylib.h"

#include <algorithm>

namespace {

void drawCentered(const char* text, int y, int fontSize, Color color)
{
    const int width = MeasureText(text, fontSize);
    DrawText(text, (cfg::screenWidth - width) / 2, y, fontSize, color);
}

void drawDimOverlay()
{
    DrawRectangle(0, 0, cfg::screenWidth, cfg::screenHeight, Fade(BLACK, cfg::overlayDim));
}

void drawKeyIcon(int x, int y)
{
    const Color gold{244, 204, 64, 255};
    DrawCircle(x + 5, y + 7, 5.0f, gold);
    DrawRectangle(x + 8, y + 5, 14, 4, gold);
    DrawRectangle(x + 18, y + 8, 3, 5, gold);
}

}

namespace hud {

void drawHud(int health, int ammo, bool hasKey, int level, int kills, float elapsedTime)
{
    const int top = cfg::mapHeight;
    const int left = static_cast<int>(cfg::hudPadding);
    const int barTop = top + 28;
    const int textTop = top + 6;
    const Color background{17, 17, 21, 255};
    const Color border{54, 54, 64, 255};
    const Color labelColor{150, 152, 162, 255};
    const Color valueColor{236, 238, 244, 255};

    DrawRectangle(0, top, cfg::screenWidth, cfg::hudHeight, background);
    DrawRectangle(0, top, cfg::screenWidth, 2, border);

    DrawText("HEALTH", left, textTop, cfg::hudLabelFontSize, labelColor);

    const float healthFraction = std::clamp(static_cast<float>(health) / static_cast<float>(cfg::playerMaxHealth), 0.0f, 1.0f);
    Color healthColor{72, 196, 104, 255};
    if (healthFraction <= 0.25f) {
        healthColor = Color{218, 64, 64, 255};
    } else if (healthFraction <= 0.5f) {
        healthColor = Color{224, 156, 54, 255};
    }

    DrawRectangle(left, barTop, static_cast<int>(cfg::healthBarWidth), static_cast<int>(cfg::healthBarHeight), Color{40, 40, 48, 255});
    DrawRectangle(left, barTop, static_cast<int>(cfg::healthBarWidth * healthFraction), static_cast<int>(cfg::healthBarHeight), healthColor);
    DrawRectangleLines(left, barTop, static_cast<int>(cfg::healthBarWidth), static_cast<int>(cfg::healthBarHeight), border);
    DrawText(TextFormat("%d", health), left + static_cast<int>(cfg::healthBarWidth) + 10, barTop - 4, cfg::hudValueFontSize, valueColor);

    const int ammoX = 310;
    DrawText("AMMO", ammoX, textTop, cfg::hudLabelFontSize, labelColor);
    DrawText(TextFormat("%d", ammo), ammoX, barTop - 4, cfg::hudValueFontSize, valueColor);

    const int keyX = 420;
    if (hasKey) {
        DrawText("KEY", keyX, textTop, cfg::hudLabelFontSize, Color{244, 204, 64, 255});
        drawKeyIcon(keyX, barTop);
    }

    const int levelX = 520;
    DrawText("LEVEL", levelX, textTop, cfg::hudLabelFontSize, labelColor);
    DrawText(TextFormat("%d/%d", level, cfg::levelCount), levelX, barTop - 4, cfg::hudValueFontSize, valueColor);

    const int right = cfg::screenWidth - static_cast<int>(cfg::hudPadding);
    const int totalSeconds = static_cast<int>(elapsedTime);
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;
    const char* killsText = TextFormat("KILLS %d", kills);
    const char* timeText = TextFormat("TIME %02d:%02d", minutes, seconds);

    DrawText(killsText, right - 230, barTop - 2, 20, valueColor);
    DrawText(timeText, right - MeasureText(timeText, 20), barTop - 2, 20, valueColor);
}

void drawMenu(bool hasError, const std::string& error)
{
    drawDimOverlay();
    drawCentered("DEMON SHOOTER", 118, cfg::titleFontSize, Color{228, 64, 64, 255});
    drawCentered("A TOP-DOWN DEMON SHOOTER", 190, cfg::menuFontSize, Color{198, 200, 210, 255});

    if (hasError) {
        drawCentered("LEVEL ERROR", 268, 30, Color{240, 92, 92, 255});
        drawCentered(error.c_str(), 312, 18, Color{240, 240, 244, 255});
        drawCentered("Fix the level file, then press Enter to retry.", 346, 18, Color{176, 178, 188, 255});
    } else {
        drawCentered("PRESS ENTER TO START", 278, 30, Color{240, 240, 244, 255});
        drawCentered("WASD move   |   Mouse aim   |   Hold left click to fire", 338, 18, Color{176, 178, 188, 255});
        drawCentered("Find the keycard, open the door, reach the exit.", 372, 18, Color{176, 178, 188, 255});
    }

    drawCentered("Esc quits from this menu", 456, 16, Color{138, 140, 150, 255});
}

void drawPaused()
{
    drawDimOverlay();
    drawCentered("PAUSED", 176, cfg::overlayFontSize, Color{240, 240, 244, 255});
    drawCentered("Esc to resume", 268, 24, Color{198, 200, 210, 255});
    drawCentered("R to restart level", 308, 24, Color{198, 200, 210, 255});
}

void drawGameOver()
{
    drawDimOverlay();
    drawCentered("GAME OVER", 176, cfg::overlayFontSize, Color{230, 62, 62, 255});
    drawCentered("R to restart", 268, 24, Color{214, 216, 224, 255});
    drawCentered("Esc to return to the menu", 308, 24, Color{176, 178, 188, 255});
}

void drawLevelComplete(int level, int kills, float elapsedTime)
{
    const int totalSeconds = static_cast<int>(elapsedTime);
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;

    drawDimOverlay();
    drawCentered("LEVEL COMPLETE", 152, cfg::overlayFontSize, Color{82, 214, 126, 255});
    drawCentered(TextFormat("Level %d of %d", level, cfg::levelCount), 214, 24, Color{198, 200, 210, 255});
    drawCentered(TextFormat("Kills: %d    Time: %02d:%02d", kills, minutes, seconds), 258, 26, Color{232, 234, 240, 255});
    drawCentered("Enter for the next level", 322, 22, Color{198, 200, 210, 255});
    drawCentered("R to replay this level    |    Esc for the menu", 358, 22, Color{176, 178, 188, 255});
}

void drawAllLevelsComplete(int kills, float elapsedTime)
{
    const int totalSeconds = static_cast<int>(elapsedTime);
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;

    drawDimOverlay();
    drawCentered("ALL LEVELS COMPLETE", 148, cfg::overlayFontSize, Color{244, 204, 64, 255});
    drawCentered(TextFormat("You fought through all %d levels.", cfg::levelCount), 214, 24, Color{198, 200, 210, 255});
    drawCentered(TextFormat("Total kills: %d    Total time: %02d:%02d", kills, minutes, seconds), 258, 26, Color{232, 234, 240, 255});
    drawCentered("Enter or R to start a new run", 322, 22, Color{198, 200, 210, 255});
    drawCentered("Esc to return to the menu", 358, 22, Color{176, 178, 188, 255});
}

}
