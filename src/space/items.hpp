#ifndef ITEMS_H
#define ITEMS_H

#include "../global.hpp"
#include <cmath>

namespace space_loop
{
    void loadItemTextures();
    void unloadItemTextures();
    GameScreen drawItems(int sectorX, int sectorY, float drawX, float drawY, const Point& globalSector);
}

#endif
