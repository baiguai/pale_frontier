#ifndef ITEMS_H
#define ITEMS_H

#include "../global.hpp"
#include <cmath>

namespace space_loop
{
    void loadItemTextures();
    void unloadItemTextures();
    GameScreen drawItems();
    void configureItem(int sector_x, int sector_y);
}

#endif
