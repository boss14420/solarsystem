/*
 * =====================================================================================
 *
 *       Filename:  planetdata.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2013 11:10:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PLANETDATA_HH__
#define __PLANETDATA_HH__

#include "config.hh"
#include <vector>

struct PlanetData {
    Float radius, semimajor_axis, eccentricity;
    Float siderial_year, siderial_day;
    Float orbit_inclination, axis_inclination;
    Float asc_node, arg_periapsis;
    Float orbitPHI; // phi in radians, phase in degrees
    char const *texture_file;
};

extern PlanetData sun_data;
#define NUM_PLANETS 2
extern std::vector<PlanetData> planets_data;
extern PlanetData moon_data;

#endif // __PLANETDATA_HH__
