/*
 * =====================================================================================
 *
 *       Filename:  planetdata.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2013 11:14:14 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "planetdata.hh"
#include "../constants.h"

const PlanetData sun_data = {
      EARTH_RADIUS * 10,   0.0f,  0.0f,   1.0f,          25.0f, 0.0f,  0.0f,                   0.0f,   0.0f,  -M_PI, "textures/sun"};

const std::vector<PlanetData> planets_data = {
    { EARTH_RADIUS,        1.0f,  0.016f, SIDERIAL_YEAR, 1.0f,  0.0f,  EARTH_AXIS_INCLINATION, 0.0f,   0.0f,  -M_PI, "textures/earth" }, // EARTH 
    { EARTH_RADIUS * 0.53, 1.52f, 0.09f,  686.9f,        1.02f, 1.85f, 25.19f,                 49.5f,  286.5, -M_PI, "textures/mars" } // Mars
};

const PlanetData moon_data = {
    MOON_RADIUS,           MOON_ORBIT_RADIUS, 0.05f, SIDERIAL_MONTH, SIDERIAL_MONTH, MOON_INCLINATION, 6.68f, 0.0f, 0.0f, 0.0f, "textures/moon"
};
