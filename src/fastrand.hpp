#ifndef FASTRAND_H
#define FASTRAND_H

#include <cstdint>

namespace Frand
{

    struct FastRand
    {
        uint64_t seed;
        uint32_t rnd();
        int randInteger(int min, int max);
        double randDouble(double min, double max);
    };

    uint64_t PerfectHash(uint32_t a, uint32_t b, uint64_t seed);

}

#endif
