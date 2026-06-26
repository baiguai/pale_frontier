#ifndef CLOUDS_H
#define CLOUDS_H

#include "../global.hpp"
#include <string>
#include <vector>

namespace surface_loop
{
    void loadCloudTexture(int planet_index);
    void unloadCloudTexture();
    void drawClouds();
}

#endif
