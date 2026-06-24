#include "../main.hpp"

namespace space_loop
{
    float rotation { 0.0f };

    GameScreen runGameLoop()
    {
        loadPlanetTextures();
        loadStarTextures();
        loadPlayerTexture();
        GameScreen next_screen { GameScreen::SPACE };

        while (true)
        {
            if (IsKeyPressed(KEY_Q))
            {
                return GameScreen::QUIT;
            }

            BeginDrawing();

                ClearBackground(BLACK);

                next_screen = drawSpace();

                if (next_screen != GameScreen::SPACE)
                {
                    return next_screen;
                }

                float moveAmount = space_speed * 60.0f * GetFrameTime();

                if (IsKeyDown(KEY_M) || IsKeyDown(KEY_DOWN))
                {
                    rotation = 180.0f;
                    space_camera.y += moveAmount;
                }
                if (IsKeyDown(KEY_I) || IsKeyDown(KEY_UP))
                {
                    rotation = 0.0f;
                    space_camera.y -= moveAmount;
                }
                if (IsKeyDown(KEY_J) || IsKeyDown(KEY_LEFT))
                {
                    rotation = 270.0f;
                    space_camera.x -= moveAmount;
                }
                if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT))
                {
                    rotation = 90.0f;
                    space_camera.x += moveAmount;
                }

            EndDrawing();
        }

        unloadPlanetTextures();
        unloadStarTextures();
        unloadPlayerTexture();

        return GameScreen::SPACE;
    }

    GameScreen drawSpace()
    {
        GameScreen next_scr { GameScreen::SPACE };

        drawStars();
        next_scr = drawPlanets();
        drawPlayer(rotation);

        return next_scr;
    }
}
