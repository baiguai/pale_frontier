#include "../main.hpp"
#include <filesystem>
namespace fs = std::filesystem;

namespace surface_loop
{
    float rotation { 0.0f };

    static double elevation_deep_water;
    static double elevation_water;
    static double elevation_walkable;
    static double elevation_shore;
    static double elevation_marsh;
    static double elevation_forest;
    static double elevation_grassland;
    static double elevation_rocky;
    static double elevation_mountain_foot;
    static double elevation_high_mountain;

    static Vector2 old_pos = surface_camera;

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


    static int surfaceConfigCount()
    {
        static int count = 0;
        if (count == 0)
        {
            for (const auto& entry : fs::directory_iterator("data/surface/config/"))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".json")
                    count++;
            }
            count = std::max(count, 1);
        }
        return count;
    }

    static void loadElevations(int variantIdx)
    {
        std::string num = variantIdx < 9 ? "0" + std::to_string(variantIdx + 1) : std::to_string(variantIdx + 1);
        std::string path = "data/surface/config/surface_" + num + ".json";

        std::ifstream in(path);
        if (!in.good())
        {
            elevation_deep_water    = 0.15;
            elevation_water         = 0.22;
            elevation_walkable      = 0.221;
            elevation_shore         = 0.40;
            elevation_marsh         = 0.52;
            elevation_forest        = 0.55;
            elevation_grassland     = 0.60;
            elevation_rocky         = 0.67;
            elevation_mountain_foot = 0.72;
            elevation_high_mountain = 0.80;
            return;
        }

        json j;
        in >> j;

        if (j.contains("elevations"))
        {
            auto& e = j["elevations"];
            elevation_deep_water    = e.value("deep_water", 0.15);
            elevation_water         = e.value("water", 0.22);
            elevation_walkable      = e.value("walkable", 0.221);
            elevation_shore         = e.value("shore", 0.40);
            elevation_marsh         = e.value("marsh", 0.52);
            elevation_forest        = e.value("forest", 0.55);
            elevation_grassland     = e.value("grassland", 0.60);
            elevation_rocky         = e.value("rocky", 0.67);
            elevation_mountain_foot = e.value("mountain_foot", 0.72);
            elevation_high_mountain = e.value("high_mountain", 0.80);
        }
    }

    struct Port {
        int x;
        int y;
    };
    inline std::vector<Port> surface_ports;
    inline Texture2D portTexture;

    void loadPortTexture()
    {
        std::string img = "data/surface/assets/port.png";
        if (fs::exists(img))
            portTexture = LoadTexture(img.c_str());
    }

    void unloadPortTexture()
    {
        UnloadTexture(portTexture);
    }

    static void savePorts(const std::string& path)
    {
        json j;
        std::ifstream in(path);
        if (in.good()) in >> j;

        json arr = json::array();
        for (const auto& p : surface_ports) {
            json obj; obj["x"] = p.x; obj["y"] = p.y;
            arr.push_back(obj);
        }
        j["ports"] = arr;

        std::ofstream out(path);
        if (out.good()) out << j.dump(2) << std::endl;
    }

    static void loadPorts(const std::string& path)
    {
        std::ifstream in(path);
        if (!in.good()) return;
        json j; in >> j;
        if (!j.contains("ports")) return;

        for (const auto& o : j["ports"])
            surface_ports.push_back({ o["x"], o["y"] });
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

    static Vector2 findLandSpawn(uint64_t seed, int maxRange, int centerX = 0, int centerY = 0)
    {
        int halfTilesX = GetScreenWidth() / 2 / surface_sector_size;
        int halfTilesY = GetScreenHeight() / 2 / surface_sector_size;

        for (int r = 0; r <= maxRange; r++) {
            // Top edge: (x, centerY - r)
            for (int x = -r; x <= r; x++) {
                int psx = centerX + x;             // player sector X
                int psy = centerY - r;             // player sector Y
                double wx = psx * surface_sector_size * surface_zoom;
                double wy = psy * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= elevation_walkable)
                    return Vector2{ (float)(psx - halfTilesX),
                                    (float)(psy - halfTilesY) };
            }
            // Bottom edge: (x, centerY + r)
            for (int x = -r; x <= r; x++) {
                int psx = centerX + x;
                int psy = centerY + r;
                double wx = psx * surface_sector_size * surface_zoom;
                double wy = psy * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= elevation_walkable)
                    return Vector2{ (float)(psx - halfTilesX),
                                    (float)(psy - halfTilesY) };
            }
            // Left edge: (centerX - r, y)  — skip corners
            for (int y = -r + 1; y < r; y++) {
                int psx = centerX - r;
                int psy = centerY + y;
                double wx = psx * surface_sector_size * surface_zoom;
                double wy = psy * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= elevation_walkable)
                    return Vector2{ (float)(psx - halfTilesX),
                                    (float)(psy - halfTilesY) };
            }
            // Right edge: (centerX + r, y) — skip corners
            for (int y = -r + 1; y < r; y++) {
                int psx = centerX + r;
                int psy = centerY + y;
                double wx = psx * surface_sector_size * surface_zoom;
                double wy = psy * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, seed) >= elevation_walkable)
                    return Vector2{ (float)(psx - halfTilesX),
                                    (float)(psy - halfTilesY) };
            }
        }
        return Vector2{ (float)centerX, (float)centerY }; // fallback
    }

    static Vector2 findLand(uint64_t seed, const int itemX, const int itemY)
    {
        int dirs[4][2] = {{10,0}, {-10,0}, {0,10}, {0,-10}};

        for (auto& d : dirs) {
            int tx = itemX + d[0];
            int ty = itemY + d[1];
            double wx = tx * surface_sector_size * surface_zoom;
            double wy = ty * surface_sector_size * surface_zoom;
            if (sampleHeight(wx, wy, seed) >= elevation_walkable)
                return Vector2{ (float)tx, (float)ty };
        }

        return Vector2{ (float)itemX, (float)itemY };
    }

    static Vector2 spawnNearPort(uint64_t seed, const Port& port)
    {
        int halfTilesX = GetScreenWidth() / 2 / surface_sector_size;
        int halfTilesY = GetScreenHeight() / 2 / surface_sector_size;

        Vector2 landLoc = findLand(seed, port.x, port.y);
        if (landLoc.x != port.x && landLoc.y != port.y)
        {
            landLoc.x = (float)(landLoc.x - halfTilesX);
            landLoc.y = (float)(landLoc.y - halfTilesY);
            return landLoc;
        }

        return findLandSpawn(seed, 20, port.x, port.y); // fallback
    }




    GameScreen runGameLoop()
    {
        loadPlayerTexture();
        loadPortTexture();
        int p_x = static_cast<int>(floor(current_planet.x));
        int p_y = static_cast<int>(floor(current_planet.y));

        uint64_t planetSeed = Frand::PerfectHash(p_x, p_y);

        frand.seed = planetSeed;
        frand.randInteger(0, planet_distance);
        int planetIndex = frand.randInteger(0, surfaceConfigCount());
        loadElevations(planetIndex);

        bool config_exists = fileExists(getPlanetPath(p_x, p_y));
        if (!config_exists) {
            surface_camera = findLandSpawn(planetSeed, 200);
            int halfTilesX = GetScreenWidth() / 2 / surface_sector_size;
            int halfTilesY = GetScreenHeight() / 2 / surface_sector_size;
            int playerTileX = static_cast<int>(floor(surface_camera.x)) + halfTilesX;
            int playerTileY = static_cast<int>(floor(surface_camera.y)) + halfTilesY;

            // Port p = { playerTileX + 10, playerTileY };
            Port p = { playerTileX, playerTileY };
            Vector2 landLoc = findLand(planetSeed, p.x, p.y);
            p.x = landLoc.x + 10;
            p.y = landLoc.y;

            // int dirs[4][2] = {{10,0}, {-10,0}, {0,10}, {0,-10}};
            // for (auto& d : dirs) {
            //     int tx = playerTileX + d[0];
            //     int ty = playerTileY + d[1];
            //     double wx = tx * surface_sector_size * surface_zoom;
            //     double wy = ty * surface_sector_size * surface_zoom;
            //     if (sampleHeight(wx, wy, planetSeed) >= elevation_walkable) {
            //         p = { tx, ty };
            //         break;
            //     }
            // }

            {
                double wx = p.x * surface_sector_size * surface_zoom;
                double wy = p.y * surface_sector_size * surface_zoom;
                if (sampleHeight(wx, wy, planetSeed) < elevation_walkable) {
                    auto pos = findLandSpawn(planetSeed, 20, playerTileX, playerTileY);
                    Vector2 landSector = { pos.x + (float)(GetScreenWidth() / 2 / surface_sector_size),
                                           pos.y + (float)(GetScreenHeight() / 2 / surface_sector_size) };
                    p = { (int)landSector.x, (int)landSector.y };
                }
            }
            surface_ports = { p };
            savePorts(getPlanetPath(p_x, p_y));
        }
        else {
            loadPorts(getPlanetPath(p_x, p_y));
            if (!surface_ports.empty())
                surface_camera = spawnNearPort(planetSeed, surface_ports[0]);
        }

        while (true)
        {
            if (IsKeyPressed(KEY_Q))
            {
                return GameScreen::QUIT;
            }

            BeginDrawing();

                ClearBackground(GREEN);

                float moveAmount = surface_speed * 60.0f * GetFrameTime();
                old_pos.x = surface_camera.x;
                old_pos.y = surface_camera.y;

                if (IsKeyDown(KEY_M) || IsKeyDown(KEY_DOWN))
                {
                    rotation = 180.0f;
                    surface_camera.y += moveAmount;
                }
                if (IsKeyDown(KEY_I) || IsKeyDown(KEY_UP))
                {
                    rotation = 0.0f;
                    surface_camera.y -= moveAmount;
                }
                if (IsKeyDown(KEY_J) || IsKeyDown(KEY_LEFT))
                {
                    rotation = 270.0f;
                    surface_camera.x -= moveAmount;
                }
                if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT))
                {
                    rotation = 90.0f;
                    surface_camera.x += moveAmount;
                }

                {
                    int cur_p_x = static_cast<int>(floor(current_planet.x));
                    int cur_p_y = static_cast<int>(floor(current_planet.y));
                    uint64_t ps = Frand::PerfectHash(cur_p_x, cur_p_y);
                    int cxi = static_cast<int>(floor(surface_camera.x));
                    int cyi = static_cast<int>(floor(surface_camera.y));
                    int ctx = (GetScreenWidth() / 2) / surface_sector_size;
                    int cty = (GetScreenHeight() / 2) / surface_sector_size;
                    double wx = (cxi + ctx) * surface_sector_size * surface_zoom;
                    double wy = (cyi + cty) * surface_sector_size * surface_zoom;
                    double h = sampleHeight(wx, wy, ps);
                    if (h < 0.0) h = 0.0;
                    if (h > 1.0) h = 1.0;
                    if (h < elevation_walkable) {
                        surface_camera.x = old_pos.x;
                        surface_camera.y = old_pos.y;
                    }
                }

                drawSurface();

                int playerSectorX = static_cast<int>(floor(surface_camera.x)) + GetScreenWidth() / 2 / surface_sector_size;
                int playerSectorY = static_cast<int>(floor(surface_camera.y)) + GetScreenHeight() / 2 / surface_sector_size;
                
                for (const auto& port : surface_ports) {
                    int dx = playerSectorX - port.x;
                    int dy = playerSectorY - port.y;
                    if (dx * dx + dy * dy < 16) {
                        return GameScreen::SPACE;
                    }
                }

            EndDrawing();
        }

        unloadPlayerTexture();
        unloadPortTexture();

        return GameScreen::SURFACE;
    }

    void drawSurface()
    {
        generateSurface();
        int cam_x_int = static_cast<int>(floor(surface_camera.x));
        int cam_y_int = static_cast<int>(floor(surface_camera.y));

        for (const auto& port : surface_ports) {
            float px = (port.x - cam_x_int) * surface_sector_size - 32;
            float py = (port.y - cam_y_int) * surface_sector_size - 32;
            DrawTexture(portTexture, (int)px, (int)py, WHITE);
        }
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
