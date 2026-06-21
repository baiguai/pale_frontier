#include "../main.hpp"

namespace surface_loop
{
    float rotation { 0.0f };

    GameScreen runGameLoop()
    {
        loadPlayerTexture();

        while (true)
        {
            if (IsKeyPressed(KEY_Q))
            {
                return GameScreen::QUIT;
            }

            BeginDrawing();

                ClearBackground(GREEN);

                drawSurface();

                float moveAmount = surface_speed * 60.0f * GetFrameTime();

                if (IsKeyDown(KEY_C))
                {
                    rotation = 180.0f;
                    surface_camera.y += moveAmount;
                }
                if (IsKeyDown(KEY_E))
                {
                    rotation = 0.0f;
                    surface_camera.y -= moveAmount;
                }
                if (IsKeyDown(KEY_S))
                {
                    rotation = 270.0f;
                    surface_camera.x -= moveAmount;
                }
                if (IsKeyDown(KEY_F))
                {
                    rotation = 90.0f;
                    surface_camera.x += moveAmount;
                }

            EndDrawing();
        }

        unloadPlayerTexture();

        return GameScreen::SURFACE;
    }

    void drawSurface()
    {
        drawPlayer(rotation);
        generateSurface();
    }

    void generateSurface()
    {

    }
}
