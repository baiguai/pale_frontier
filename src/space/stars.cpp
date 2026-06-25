#include "stars.hpp"

namespace space_loop
{
    static std::vector<Texture2D> starTextures;

    void loadStarTextures()
    {
        starTextures.clear();
        const char* names[] = { "stars_01.png", "stars_02.png", "stars_03.png" };
        for (const char* name : names) {
            std::string path = std::string("data/space/assets/stars/") + name;
            starTextures.push_back(LoadTexture(path.c_str()));
        }
    }

    void unloadStarTextures()
    {
        for (auto& tex : starTextures)
            UnloadTexture(tex);
        starTextures.clear();
    }

    void drawStars()
    {
        if (starTextures.size() < 3) return;

        struct { Texture2D& tex; float speed; } layers[] = {
            { starTextures[0], star_speed_01 },
            { starTextures[1], star_speed_02 },
            { starTextures[2], star_speed_03 },
        };

        for (auto& l : layers) {
            float ox = -fmodf(space_camera.x * l.speed, (float)l.tex.width);
            float oy = -fmodf(space_camera.y * l.speed, (float)l.tex.height);
            if (ox > 0) ox -= (float)l.tex.width;
            if (oy > 0) oy -= (float)l.tex.height;

            int tilesX = GetScreenWidth()  / l.tex.width  + 2;
            int tilesY = GetScreenHeight() / l.tex.height + 2;

            for (int tx = 0; tx < tilesX; tx++)
                for (int ty = 0; ty < tilesY; ty++)
                    DrawTexture(l.tex,
                                ox + tx * l.tex.width,
                                oy + ty * l.tex.height, WHITE);
        }
    }
}
