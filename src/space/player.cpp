#include "player.hpp"
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

namespace space_loop
{
    Texture2D playerTexture;

    void loadPlayerTexture()
    {
        std::string img = "data/space/assets/player.png";
        if (!fs::exists(img))
        {
            std::cout << "ERROR:: The player image does not exist.\n";
            return;
        }

        playerTexture = LoadTexture(img.c_str());
    }

    void unloadPlanetTexture()
    {
        UnloadTexture(playerTexture);
    }

    void drawPlayer(float rotation)
    {
        float draw_x = (GetScreenWidth() / 2) - 12;
        float draw_y = (GetScreenHeight() / 2) - 12;

        Rectangle sourceRec = { 0.0f, 0.0f, 24.0f, 24.0f };
        Rectangle destRec = { draw_x + 12, draw_y + 12, 24.0f, 24.0f };
        Vector2 origin = { 12.0f, 12.0f };

        DrawTexturePro(playerTexture, sourceRec, destRec, origin, rotation, WHITE);

        // DrawTexture(playerTexture, draw_x, draw_y, WHITE);
    }
}
