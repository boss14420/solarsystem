/*
 * =====================================================================================
 *
 *       Filename:  solarsystem.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2013 02:06:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "solarsystem.hh"
//#include "../common/shader.hpp"

//////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_OPENGLES
    #define PLANET_FRAG_SHADER "shaders/sphere.gl.frag"
    #define PLANET_VERT_SHADER "shaders/sphere.gl.vert"
    #define STAR_FRAG_SHADER "shaders/star.gl.frag"
    #define STAR_VERT_SHADER "shaders/star.gl.vert"
#else
    #define PLANET_FRAG_SHADER "shaders/sphere.gl.frag"
    #define PLANET_VERT_SHADER "shaders/sphere.gl.vert"
    #define STAR_FRAG_SHADER "shaders/star.gl.frag"
    #define STAR_VERT_SHADER "shaders/star.gl.vert"
#endif
#define SPHERE_MODEL "objects/earth.obj"
#define SIMPLE_FRAG_SHADER "shaders/simple.gl.frag"
#define SIMPLE_VERT_SHADER "shaders/simple.gl.vert"
///////////////////////////////////////////////////////////////////////////////////////


#ifdef USE_HALFFLOAT
    #define DEFINITION "#define USE_HALFFLOAT"
#else
    #define DEFINITION ""
#endif
SolarSystem::SolarSystem()
    : _model_matrix (1.0f),
      _star_shader_program(STAR_VERT_SHADER, STAR_FRAG_SHADER, DEFINITION),
      _planet_shader_program(PLANET_VERT_SHADER, PLANET_FRAG_SHADER, DEFINITION),
      _orbit_shader_program(SIMPLE_VERT_SHADER, SIMPLE_FRAG_SHADER, DEFINITION)
{
    Planet::_sphere_model.load_model(SPHERE_MODEL);
    Planet::_orbit_model.generate(200, 1.0);
}
#undef DEFINITION


mat4 SolarSystem::render (mat4 model_matrix, mat4 view_matrix, mat4 mvp_matrix, bool orbit) const
{
    model_matrix *= _model_matrix;
    mvp_matrix *= _model_matrix;

    _star_shader_program.use();
    Star::_sphere_model.use_program(_star_shader_program);
    Star::_sphere_model.prepare_render(model_matrix, view_matrix, mvp_matrix);
    mvp_matrix = _sun.render(model_matrix, mvp_matrix);

    _planet_shader_program.use();
    Planet::_sphere_model.use_program(_planet_shader_program);
    Planet::_sphere_model.prepare_render(model_matrix, view_matrix, mvp_matrix);
    Planet::_sphere_model.set_light_position(0, 0, 0);
    for (auto const &planet : _planet_list) {
        planet.render(model_matrix, mvp_matrix);
    }

//    _sun.moons[1].render(mvp_matrix);
//    _sun.moons[0].render(mvp_matrix);

//    mvp_matrix = _sun.render(mvp_matrix);
//    for (auto const &planet : _planet_list)
//        planet.render(mvp_matrix);

    _orbit_shader_program.use();
    Planet::_orbit_model.use_program(_orbit_shader_program);
    Planet::_orbit_model.prepare_render(model_matrix, view_matrix, mvp_matrix);
    if (orbit) {
        for (auto const &planet : _planet_list)
            planet.render_orbit(mvp_matrix);
    }

    return mvp_matrix;
}

void SolarSystem::move (int elapse)
{
    _sun.move(elapse);
    for (auto &planet : _planet_list)
        planet.move(elapse);
}
