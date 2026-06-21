#ifndef PLAYER_SURFACE_H
#define PLAYER_SURFACE_H

#include "../global.hpp"
#include <cmath>

namespace surface_loop
{
    void loadPlayerTexture();
    void unloadPlayerTexture();
    void drawPlayer(float rotation);
}

#endif
