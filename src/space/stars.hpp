#ifndef STARS_H
#define STARS_H

#include "../global.hpp"
#include <cmath>

namespace space_loop
{
    void drawStars();
    void drawStars2();
    void drawStars3();
    void renderStars(Color star_color, int sector_size, int star_distance, int sec_num_x, int sec_num_y, float radius);
}

#endif
