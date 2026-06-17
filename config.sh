#!/bin/bash

# Central configuration - edit values here, all scripts pick them up

APP_NAME="pale_frontier"

SOURCES=(
    "src/main.cpp"
    "src/fastrand.cpp"
)

LIBS=(
    "libs/raylib/libraylib.a"
)

HEADERS=(
    "src/main.hpp"
    "src/fastrand.hpp"
    "libs/raylib/raylib.h"
    "libs/raylib/raymath.h"
    "libs/raylib/rlgl.h"
)
