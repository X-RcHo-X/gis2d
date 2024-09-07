#ifndef CONSTANTS_H
#define CONSTANTS_H

//Qt
#include <QtMath>

namespace constant
{
    namespace earth
    {
        //Размер тайла
        constexpr int TILE_SIZE = 256;

        //Радиус Земли
        constexpr double EARTH_RADIUS = 6378137.0;

        //Половина длины экватора
        constexpr double ORIGIN_SHIFT = 2.0 * M_PI * EARTH_RADIUS / 2.0;

        //Кол-во тайлов, которое может помеситься на экваторе Земли
        constexpr double INITIAL_RESOLUTION = 2.0 * M_PI * EARTH_RADIUS / TILE_SIZE;

        //Кол-во радиан в градусе (pi/180)
        constexpr double MATH_RAD_IN_GRAD = 0.017453292519943295;

        //Кол-во градусов в радиане (180/pi)
        constexpr double MATH_GRAD_IN_RAD = 57.295779513082323;
    }
}

#endif //CONSTANTS_H