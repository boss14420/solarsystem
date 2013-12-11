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
    #define STAR_VERT_SHADER "shaders/sphere.gl.vert"
#else
    #define PLANET_FRAG_SHADER "shaders/sphere.gl.frag"
    #define PLANET_VERT_SHADER "shaders/sphere.gl.vert"
    #define STAR_FRAG_SHADER "shaders/star.gl.frag"
    #define STAR_VERT_SHADER "shaders/sphere.gl.vert"
#endif
#define SPHERE_MODEL "objects/earth.obj"
#define SIMPLE_FRAG_SHADER "shaders/simple.gl.frag"
///////////////////////////////////////////////////////////////////////////////////////


SolarSystem::SolarSystem()
    : _model_matrix (1.0f)//, _display_orbit(false)
{
//    _shader_program = LoadShaders("vertex.glsl", "frag.glsl");
//    Model::load_model("earth.obj", _shader_program);

//    Planet::_sphere_model.load_model(SPHERE_MODEL, PLANET_VERT_SHADER, PLANET_FRAG_SHADER);
//    Star::_sphere_model.load_model(SPHERE_MODEL, STAR_VERT_SHADER, STAR_FRAG_SHADER);
    Planet::_sphere_model.load_model(SPHERE_MODEL);
    Planet::_orbit_model.generate(200, 1.0);

#ifdef USE_HALFFLOAT
    #define DEFINITION "#define USE_HALFFLOAT"
#else
    #define DEFINITION ""
#endif
    Shader planet_vertex_shader(GL_VERTEX_SHADER, PLANET_VERT_SHADER, DEFINITION),
           planet_frag_shader(GL_FRAGMENT_SHADER, PLANET_FRAG_SHADER, DEFINITION),
           star_frag_shader(GL_FRAGMENT_SHADER, STAR_FRAG_SHADER, DEFINITION),
           orbit_frag_shader(GL_FRAGMENT_SHADER, SIMPLE_FRAG_SHADER, DEFINITION);
#undef DEFINITION

    _star_shader_program.link(planet_vertex_shader, star_frag_shader);
    _planet_shader_program.link(planet_vertex_shader, planet_frag_shader);
    _orbit_shader_program.link(planet_vertex_shader, orbit_frag_shader);
}


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
