#!/bin/bash

# Central configuration - edit values here, all scripts pick them up

APP_NAME="pale_frontier"

SOURCES=(
    "src/main.cpp"
    "src/space/spaceloop.cpp"
    "src/surface/surfaceloop.cpp"
    "src/space/planets.cpp"
    "src/space/stars.cpp"
    "src/space/player.cpp"
    "src/fastrand.cpp"
)

LIBS=(
    "libs/raylib/libraylib.a"
)

HEADERS=(
    "src/config.hpp"
    "src/fileio.hpp"
    "src/global.hpp"
    "src/main.hpp"
    "src/space/spaceloop.hpp"
    "src/surface/surfaceloop.hpp"
    "src/space/planets.hpp"
    "src/space/stars.hpp"
    "src/space/player.hpp"
    "src/fastrand.hpp"
    "libs/raylib/raylib.h"
    "libs/nlohmann/json.hpp"
    "libs/raylib/raymath.h"
    "libs/raylib/rlgl.h"
)
