#!/bin/bash

# Central configuration - edit values here, all scripts pick them up

APP_NAME="pale_frontier"

SOURCES=(
    "src/main.cpp"
    "src/planets.cpp"
    "src/stars.cpp"
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
    "src/planets.hpp"
    "src/stars.hpp"
    "src/fastrand.hpp"
    "libs/raylib/raylib.h"
    "libs/nlohmann/json.hpp"
    "libs/raylib/raymath.h"
    "libs/raylib/rlgl.h"
)
