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
 *         Author:  BOSS14420 (), ireoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "planetdata.hh"
//#include "../constants.h"

#define AU 2
#define EARTH_RADIUS (AU * 4.2805e-5) * 200
#define MOON_ORBIT_RADIUS (AU*.00257) * 50
#define ECLIPTIC_INCLINATION 7.2

    //                       Radius               A      Ecc    Year    Day      Incl   Tilt    Node    Perih.  Texture                 Phase
const PlanetData sun_data = {
      EARTH_RADIUS *  19,    9e-6,     0.0,        0.0,    1.0,     25.0, 0.0f,  0.0,                   0.0,   0.0,  -M_PI, "textures/sun"};

const std::vector<PlanetData> planets_data = {
    { EARTH_RADIUS * 0.38,   .0,       AU * 0.39f, 0.2f,   87.9f,  58.6f,   7.0f+ECLIPTIC_INCLINATION,  0.03f,  48.33f, 29.12f, -M_PI, "textures/mercury" }, // Mercury
    { EARTH_RADIUS * 0.93,   .0,       AU * 0.7f,  0.006f, 224.7f, -243.0f, 3.39f+ECLIPTIC_INCLINATION, 177.3f, 76.67f, 55.18f, -M_PI, "textures/venus" },  // Venus
    { EARTH_RADIUS,          .0033528, AU * 1.0,   0.016,  365.3,    1.0,  0.0+ECLIPTIC_INCLINATION,  23.4,   0.0,   0.0,  -M_PI, "textures/earth" }, // EARTH 
    { EARTH_RADIUS * 0.53,   .00589,   AU * 1.52,  0.09,   686.9,   1.02, 1.85+ECLIPTIC_INCLINATION, 25.19,  49.5,  286.5, -M_PI, "textures/mars" }, // Mars
    { EARTH_RADIUS * 11.209, .06487,   AU * 5.204, 0.049,  4332.59, .414,1.305+ECLIPTIC_INCLINATION,  3.13, 100.5,  275.1, -M_PI, "textures/jupiter" } // Jupiter
};

const std::vector<std::vector<PlanetData>> moons_data = {
    {  }, // Mercury
    {  }, // Venus
   {{ EARTH_RADIUS * 0.273,  .00125,   MOON_ORBIT_RADIUS,       .0549, 27.3, 27.3, 5.1, 6.68, 0.0, 0.0, 0.0, "textures/moon" }}, // Moon
//    { EARTH_RADIUS * 0.273,  .00125, 3*MOON_ORBIT_RADIUS,       .0549, 27.3, 27.3, 5.1, 6.68, 0.0, 0.0, 0.0, "textures/moon" }}, // Moon
   {{ 10*EARTH_RADIUS * 1.8e-3, 0, 10* MOON_ORBIT_RADIUS*.0244, .0151,  .319, .319, 1.09, .0,  0.0, 0.0, 0.0, "textures/phobos" }, // Phobos 
    { 10*EARTH_RADIUS *9.72e-4, 0, 10* MOON_ORBIT_RADIUS*.0610, .0003, 1.263,1.263, .93,  .0,  0.0, 0.0, 0.0, "textures/phobos" }}, // Deimos
   {{ EARTH_RADIUS * 0.286,     0, MOON_ORBIT_RADIUS*1.097,     .0041, 1.769,1.769, 0.05, 0.,   0.,  0., 0.,  "textures/io" }, // Io 
    { EARTH_RADIUS * 0.245,     0, MOON_ORBIT_RADIUS*1.7457,    .0090, 3.552,3.552, 0.47, .1,   0.,  0., 0.,  "textures/europa" }, // Europa
    { EARTH_RADIUS * 0.413,     0, MOON_ORBIT_RADIUS*2.7846,    .0013, 7.155,7.155, 0.20, .33,  0.,  0., 0.,  "textures/ganymede" }, // Ganymede
    { EARTH_RADIUS * 0.378,     0, MOON_ORBIT_RADIUS*4.8978,    .0074, 16.69,16.69, 0.00, .00,  0.,  0., 0.,  "textures/callisto" }} // Callisto
};
