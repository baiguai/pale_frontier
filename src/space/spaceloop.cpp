#include "../main.hpp"

namespace space_loop
{
    float rotation { 0.0f };

    GameScreen runGameLoop()
    {
        loadPlanetTextures();
        loadPlayerTexture();

        while (true)
        {
            if (IsKeyPressed(KEY_Q))
            {
                return GameScreen::QUIT;
            }

            BeginDrawing();

                ClearBackground(BLACK);

                drawSpace();

                float moveAmount = space_speed * 60.0f * GetFrameTime();

                if (IsKeyDown(KEY_C))
                {
                    rotation = 180.0f;
                    space_camera.y += moveAmount;
                }
                if (IsKeyDown(KEY_E))
                {
                    rotation = 0.0f;
                    space_camera.y -= moveAmount;
                }
                if (IsKeyDown(KEY_S))
                {
                    rotation = 270.0f;
                    space_camera.x -= moveAmount;
                }
                if (IsKeyDown(KEY_F))
                {
                    rotation = 90.0f;
                    space_camera.x += moveAmount;
                }

            EndDrawing();
        }

        unloadPlanetTextures();
        unloadPlayerTexture();

        return GameScreen::SPACE;
    }

    void drawSpace()
    {
        drawStars();
        drawPlanets();
        drawPlayer(rotation);
    }
}
