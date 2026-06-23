#include "../main.hpp"

namespace surface_loop
{
    float rotation { 0.0f };

    inline double elevation_deep_water    { 0.15 };
    inline double elevation_water         { 0.22 };
    inline double elevation_shore         { 0.50 };
    inline double elevation_marsh         { 0.58 };
    inline double elevation_forest        { 0.65 };
    inline double elevation_grassland     { 0.72 };
    inline double elevation_rocky         { 0.80 };
    inline double elevation_mountain_foot { 0.88 };
    inline double elevation_high_mountain { 0.95 };

    static Color heightToColor(double height)
    {
        if (height < elevation_deep_water   ) return Color{ 10,  10,  60,  255 };
        if (height < elevation_water        ) return Color{ 30,  60,  130, 255 };
        if (height < elevation_shore        ) return Color{ 210, 190, 140, 255 };
        if (height < elevation_marsh        ) return Color{ 140, 170, 110, 255 };
        if (height < elevation_forest       ) return Color{ 50,  140, 50,  255 };
        if (height < elevation_grassland    ) return Color{ 80,  170, 60,  255 };
        if (height < elevation_rocky        ) return Color{ 90,  90,  90,  255 };
        if (height < elevation_mountain_foot) return Color{ 140, 140, 140, 255 };
        if (height < elevation_high_mountain) return Color{ 190, 190, 190, 255 };
        return                                Color{ 240, 240, 240, 255 };
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

    static double sampleHeight(double worldX, double worldY, uint64_t seed)
    {
        double h0 = perlinOctave(worldX, worldY, 512, 0.6, seed);
        double h1 = perlinOctave(worldX, worldY, 128, 0.3, seed);
        double h2 = perlinOctave(worldX, worldY, 32,  0.1, seed);
        double h = (h0 + h1 + h2 + 0.5) / 1.5;
        if (h < 0.0) h = 0.0;
        if (h > 1.0) h = 1.0;
        return h;
    }

    static Vector2 findLandSpawn(uint64_t seed, int maxRange)
    {
        for (int r = 0; r <= maxRange; r++) {
            for (int x = -r; x <= r; x++) {
                double wx = x * surface_sector_size * surface_zoom;
                double wy = -r * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= 0.50)
                    return Vector2{ (float)x, (float)(-r) };
            }
            for (int x = -r; x <= r; x++) {
                double wx = x * surface_sector_size * surface_zoom;
                double wy = r * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= 0.50)
                    return Vector2{ (float)x, (float)r };
            }
            for (int y = -r + 1; y < r; y++) {
                double wx = -r * surface_sector_size * surface_zoom;
                double wy = y * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= 0.50)
                    return Vector2{ (float)(-r), (float)y };
            }
            for (int y = -r + 1; y < r; y++) {
                double wx = r * surface_sector_size * surface_zoom;
                double wy = y * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= 0.50)
                    return Vector2{ (float)r, (float)y };
            }
        }
        return Vector2{ 0, 0 }; // fallback — should never reach
    }




    GameScreen runGameLoop()
    {
        loadPlayerTexture();
        int p_x = static_cast<int>(floor(current_planet.x));
        int p_y = static_cast<int>(floor(current_planet.y));

        uint64_t planetSeed = Frand::PerfectHash(p_x, p_y);

        bool config_exists = fileExists(getPlanetPath(p_x, p_y));
        if (!config_exists) {
            surface_camera = findLandSpawn(planetSeed, 200);
        }

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
        generateSurface();
        drawPlayer(rotation);
    }

    void generateSurface()
    {
        int cur_p_x = static_cast<int>(floor(current_planet.x));
        int cur_p_y = static_cast<int>(floor(current_planet.y));
        uint64_t planetSeed = Frand::PerfectHash(cur_p_x, cur_p_y);

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

                double height = sampleHeight(worldX, worldY, planetSeed);

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
