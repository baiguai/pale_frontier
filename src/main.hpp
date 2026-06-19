#ifndef MAIN_H
#define MAIN_H 

#include "../libs/raylib/raylib.h"
#include "fastrand.hpp"
#include <iostream>
#include <cmath>
#include "main.hpp"
#include "global.hpp"
#include "planets.hpp"
#include "stars.hpp"
#include "fileio.hpp"

struct Point
{
    int x;
    int y;
};

void drawSpace();

#endif
