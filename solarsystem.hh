/*
 * =====================================================================================
 *
 *       Filename:  solarsystem.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2013 01:38:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __SOLARSYSTEM_HH__
#define __SOLARSYSTEM_HH__

#include "config.hh"
#include "planet.hh"
#include <vector>

class SolarSystem {
    Planet _sun;
//    std::vector<Planet> _planet_list;

    mat4 _model_matrix;
//    GLuint _shader_program;
    bool _display_orbit;

public:
    SolarSystem ();

    template <typename... Args>
    void set_sun(Args&&... args) {
        _sun = std::move(Planet(std::forward<Args>(args)...));
    }

    mat4 render (mat4 model_matrix, mat4 view_matrix, mat4 mvp_matrix) const;
    void move (int elapsed);

    template <typename... Args>
    Planet& add_planet (Args&&... args) 
    {
        _sun.moons.emplace_back(std::forward<Args>(args)...);
        return _sun.moons.back();
    }
};

#endif // __SOLARSYSTEM_HH__
