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




    static double smoothstep(double t)
    {
        return t * t * (3.0 - 2.0 * t);
    }

    static double lerp(double a, double b, double t)
    {
        return a + (b - a) * t;
    }

    static double perlinGrid(int gx, int gy, double dx, double dy, uint64_t seed)
    {
        frand.seed = Frand::PerfectHash(gx, gy) ^ seed;
        double angle = frand.randDouble(0.0, PI * 2.0);
        return dx * cos(angle) + dy * sin(angle);
    }

    static double perlinOctave(double worldX, double worldY, int gridSize, double amplitude, uint64_t seed)
    {
        double gx = floor(worldX / gridSize) * gridSize;
        double gy = floor(worldY / gridSize) * gridSize;
        double dx = (worldX - gx) / gridSize;
        double dy = (worldY - gy) / gridSize;
        double tx = smoothstep(dx);
        double ty = smoothstep(dy);
    
        double v00 = perlinGrid((int)gx,              (int)gy,              dx,     dy,     seed);
        double v10 = perlinGrid((int)gx + gridSize,   (int)gy,              dx-1.0, dy,     seed);
        double v01 = perlinGrid((int)gx,              (int)gy + gridSize,   dx,     dy-1.0, seed);
        double v11 = perlinGrid((int)gx + gridSize,   (int)gy + gridSize,   dx-1.0, dy-1.0, seed);
    
        return lerp(lerp(v00, v10, tx), lerp(v01, v11, tx), ty) * amplitude;
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
            static_cast<int>(floor(current_planet.x)),
            static_cast<int>(floor(current_planet.y))
        );

        int cam_x_int = static_cast<int>(floor(surface_camera.x));
        int cam_y_int = static_cast<int>(floor(surface_camera.y));
        int sec_num_x = GetScreenWidth()  / surface_sector_size + 1;
        int sec_num_y = GetScreenHeight() / surface_sector_size + 1;

        for (int x = 0; x < sec_num_x; x++)
        {
            for (int y = 0; y < sec_num_y; y++)
            {
                double worldX = (cam_x_int + x) * surface_sector_size * surface_zoom;
                double worldY = (cam_y_int + y) * surface_sector_size * surface_zoom;

                double h0 = perlinOctave(worldX, worldY, 512, 0.6, planetSeed);
                double h1 = perlinOctave(worldX, worldY, 128, 0.3, planetSeed);
                double h2 = perlinOctave(worldX, worldY, 32,  0.1, planetSeed);

                double height = (h0 + h1 + h2 + 0.5) / 1.5;
                if (height < 0.0) height = 0.0;
                if (height > 1.0) height = 1.0;

                Color color = heightToColor(height);

                DrawRectangle(
                    x * surface_sector_size,
                    y * surface_sector_size,
                    surface_sector_size,
                    surface_sector_size,
                    color
                );
            }
        }
    }
}
