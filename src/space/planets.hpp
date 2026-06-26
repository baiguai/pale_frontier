#ifndef PLANETS_H
#define PLANETS_H

#include "../global.hpp"
#include <cmath>

namespace space_loop
{
    void loadPlanetTextures();
    void unloadPlanetTextures();
    GameScreen drawPlanets();
    void configurePlanet(int sector_x, int sector_y);
}

#endif
