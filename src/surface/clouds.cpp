#include "clouds.hpp"

namespace surface_loop
{
    static Texture2D cloudTexture;

    void loadCloudTexture(int planet_index)
    {
        std::string path = "data/surface/assets/clouds/clouds_" + std::to_string(planet_index) + ".png";
        cloudTexture = LoadTexture(path.c_str());
    }

    void unloadCloudTexture()
    {
        UnloadTexture(cloudTexture);
    }

    void drawClouds()
    {
        float ox = -fmodf(surface_camera.x * cloud_speed, (float)cloudTexture.width);
        float oy = -fmodf(surface_camera.y * cloud_speed, (float)cloudTexture.height);
        if (ox > 0) ox -= (float)cloudTexture.width;
        if (oy > 0) oy -= (float)cloudTexture.height;

        int tilesX = GetScreenWidth()  / cloudTexture.width  + 2;
        int tilesY = GetScreenHeight() / cloudTexture.height + 2;

        for (int tx = 0; tx < tilesX; tx++)
            for (int ty = 0; ty < tilesY; ty++)
                DrawTexture(cloudTexture,
                            ox + tx * cloudTexture.width,
                            oy + ty * cloudTexture.height, WHITE);
    }
}
