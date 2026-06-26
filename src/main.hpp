#ifndef MAIN_H
#define MAIN_H 

struct Point
{
    int x;
    int y;
};

enum class GameScreen
{
    START_MENU,
    OPTIONS,
    SPACE,
    SURFACE,
    BASE,
    QUIT
};
inline GameScreen currentScreen { GameScreen::SPACE };

#include "../libs/raylib/raylib.h"
#include "fastrand.hpp"
#include <iostream>
#include <cmath>
#include "global.hpp"
#include "fileio.hpp"
#include "config.hpp"
#include "main.hpp"
#include "space/spaceloop.hpp"
#include "space/planets.hpp"
#include "space/stars.hpp"
#include "space/player.hpp"
#include "surface/surfaceloop.hpp"
#include "surface/player.hpp"
#include "surface/clouds.hpp"

void drawSpace();

#endif
