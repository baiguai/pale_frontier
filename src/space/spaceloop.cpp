#include "../main.hpp"

namespace space_loop
{
    GameScreen runGameLoop()
    {
        loadPlanetTextures();

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
                    space_camera.y += moveAmount;
                }
                if (IsKeyDown(KEY_E))
                {
                    space_camera.y -= moveAmount;
                }
                if (IsKeyDown(KEY_S))
                {
                    space_camera.x -= moveAmount;
                }
                if (IsKeyDown(KEY_F))
                {
                    space_camera.x += moveAmount;
                }

            EndDrawing();
        }

        unloadPlanetTextures();

        return GameScreen::SPACE;
    }

    void drawSpace()
    {
        drawStars();
        drawPlanets();
    }
}
