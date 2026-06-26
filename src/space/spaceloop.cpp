#include "../main.hpp"

namespace space_loop
{
    float rotation { 0.0f };
    int frame_count { 1 };
    int fuel_usage_count { 1 };

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

            // auto save
            if (frame_count % (3 * 60 * 60) == 0)
            {
                saveVarsToConfig();
                std::cout<<"Saved configuration.\n";
                frame_count = 0;
            }

            // fuel usage
            if (fuel_usage_count % (30 * 60) == 0)
            {
                --player_space_fuel;
                fuel_usage_count = 0;
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
                    ++fuel_usage_count;
                }
                if (IsKeyDown(KEY_I) || IsKeyDown(KEY_UP))
                {
                    rotation = 0.0f;
                    space_camera.y -= moveAmount;
                    ++fuel_usage_count;
                }
                if (IsKeyDown(KEY_J) || IsKeyDown(KEY_LEFT))
                {
                    rotation = 270.0f;
                    space_camera.x -= moveAmount;
                    ++fuel_usage_count;
                }
                if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT))
                {
                    rotation = 90.0f;
                    space_camera.x += moveAmount;
                    ++fuel_usage_count;
                }

            EndDrawing();

            ++frame_count;
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
