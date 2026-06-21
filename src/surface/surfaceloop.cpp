#include "../main.hpp"

namespace surface_loop
{
    float rotation { 0.0f };

    static Color heightToColor(double height)
    {
        if (height < 0.35) return Color{ 10,  10,  60,  255 }; // deep water
        if (height < 0.42) return Color{ 30,  60,  130, 255 }; // water
        if (height < 0.50) return Color{ 210, 190, 140, 255 }; // shore
        if (height < 0.58) return Color{ 140, 170, 110, 255 }; // marsh
        if (height < 0.65) return Color{ 50,  140, 50,  255 }; // forest
        if (height < 0.72) return Color{ 80,  170, 60,  255 }; // grassland
        if (height < 0.80) return Color{ 90,  90,  90,  255 }; // rocky
        if (height < 0.88) return Color{ 140, 140, 140, 255 }; // mountain foot
        if (height < 0.95) return Color{ 190, 190, 190, 255 }; // high mountain
        return                     Color{ 240, 240, 240, 255 }; // peaks
    }

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
        uint64_t planetSeed = Frand::PerfectHash(
            (static_cast<int>(floor(current_planet.x)) << 16) ^ static_cast<int>(floor(surface_camera.x)),
            (static_cast<int>(floor(current_planet.y)) << 16) ^ static_cast<int>(floor(surface_camera.y))
        );
    }
}
