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
#include "common/shader.hpp"

SolarSystem::SolarSystem()
    : _model_matrix (1.0f), _display_orbit(false)
{
//    _shader_program = LoadShaders("vertex.glsl", "frag.glsl");
//    Model::load_model("earth.obj", _shader_program);

    Planet::_sphere_model.load_model(SPHERE_MODEL, PLANET_VERT_SHADER, PLANET_FRAG_SHADER);
    Star::_sphere_model.load_model(SPHERE_MODEL, STAR_VERT_SHADER, STAR_FRAG_SHADER);
}


mat4 SolarSystem::render (mat4 model_matrix, mat4 view_matrix, mat4 mvp_matrix) const
{
    model_matrix *= _model_matrix;
    mvp_matrix *= _model_matrix;

    Star::_sphere_model.prepare_render(model_matrix, view_matrix, mvp_matrix);
    mvp_matrix = _sun.render(model_matrix, mvp_matrix);

    Planet::_sphere_model.prepare_render(model_matrix, view_matrix, mvp_matrix);
    for (auto const &planet : _planet_list) {
        planet.render(model_matrix, mvp_matrix);
    }

//    _sun.moons[1].render(mvp_matrix);
//    _sun.moons[0].render(mvp_matrix);

//    mvp_matrix = _sun.render(mvp_matrix);
//    for (auto const &planet : _planet_list)
//        planet.render(mvp_matrix);

//    if (_display_orbit) {
//        for (auto const &planet : _planet_list)
//            planet.render_orbit(mvp_matrix);
//    }

    return mvp_matrix;
}

void SolarSystem::move (int elapse)
{
    _sun.move(elapse);
    for (auto &planet : _planet_list)
        planet.move(elapse);
}
