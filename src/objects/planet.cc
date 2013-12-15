
// windows.h must be included before GL headers
#ifdef _MSC_VER
#include <windows.h>
#define _USE_MATH_DEFINES
#endif
#include <cmath>
//#include <GL/glew.h>
#include <GLES2/gl2.h>

//#include "../constants.h"
#include "planet.hh"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "../common/texture.hpp"


#ifndef NDEBUG
    #define FLOG(STR, ...) std::fprintf(stderr, STR, __VA_ARGS__)
#else
    #define FLOG(STR, ...)
#endif

Mesh Planet::_sphere_model;
Circle Planet::_orbit_model;

Planet::Planet(PlanetData const &pd)
  : radius(pd.radius),
    flattening(pd.flattening),
    semimajor_axis(pd.semimajor_axis),
    eccentricity(pd.eccentricity),
    siderial_year(pd.siderial_year),
    siderial_day(pd.siderial_day),
    orbit_inclination(pd.orbit_inclination),
    axis_inclination(pd.axis_inclination),
    asc_node(pd.asc_node),
    arg_periapsis(pd.arg_periapsis),
    orbitX(0),
    orbitZ(0),
    orbitPHI(pd.orbitPHI),
    phase(0),
    _texture(loadTexture(pd.texture_file)),
    _orbit_model_matrix(1.0f),
    _sphere_model_matrix(glm::scale(radius, radius*(1-flattening), radius))
{
    semiminor_axis = semimajor_axis * std::sqrt(1.0f - eccentricity*eccentricity);
//    texture = loadBMPTexture(texture_file);
//    texture = loadDDS(texture_file);

//    _orbit_model_matrix *= glm::rotate(ECLIPTIC_INCLINATION, 0.0f, 0.0f, 1.0f);

    _orbit_model_matrix *= glm::rotate(asc_node, 0.0f, 1.0f, 0.0f);
    _orbit_model_matrix *= glm::rotate(orbit_inclination, 0.0f, 0.0f, 1.0f);
    _orbit_model_matrix *= glm::rotate(arg_periapsis, 0.0f, 1.0f, 0.0f);

    orbitX = semimajor_axis * cos(-orbitPHI);
    orbitZ = semiminor_axis * sin(-orbitPHI);

    FLOG("Planet constructor, address %p, texture %u, texture file %s\n", this, _texture, pd.texture_file);
}

Planet::Planet(Planet &&rvalue) :
    radius(rvalue.radius),
    flattening(rvalue.flattening),
    semimajor_axis(rvalue.semimajor_axis),
    eccentricity(rvalue.eccentricity),
    semiminor_axis(rvalue.semiminor_axis),
    siderial_year(rvalue.siderial_year),
    siderial_day(rvalue.siderial_day),
    orbit_inclination(rvalue.orbit_inclination),
    axis_inclination(rvalue.axis_inclination),
    asc_node(rvalue.asc_node),
    arg_periapsis(rvalue.arg_periapsis),
    orbitX(rvalue.orbitX),
    orbitZ(rvalue.orbitZ),
    orbitPHI(rvalue.orbitPHI),
    phase(rvalue.phase),
    moons(std::move(rvalue.moons)),
    _orbit_model_matrix(std::move(rvalue._orbit_model_matrix)),
    _sphere_model_matrix(std::move(rvalue._sphere_model_matrix))
{
    _texture = rvalue._texture;
    rvalue._texture = 0;

    FLOG("Planet move constructor, from %p to %p, texture %u\n", &rvalue, this, _texture);

//    for (auto it = rvalue.moons.begin(); it != rvalue.moons.end(); it++)
//        moons.push_back(std::move(*it));
}

Planet& Planet::operator=(Planet &&p)
{
    if (&p == this)
        return *this;

    radius = p.radius;
    flattening = p.flattening;
    semiminor_axis = p.semimajor_axis;
    eccentricity = p.eccentricity;
    semiminor_axis = p.semiminor_axis;
    siderial_year = p.siderial_year;
    siderial_day = p.siderial_day;
    orbit_inclination = p.orbit_inclination;
    axis_inclination = p.axis_inclination;
    asc_node = p.asc_node;
    arg_periapsis = p.arg_periapsis;
    orbitX = p.orbitX;
    orbitZ = p.orbitZ;
    orbitPHI = p.orbitPHI;
    phase = p.phase;
    moons = std::move(p.moons);
    _texture = p._texture;
    p._texture = 0;
    _orbit_model_matrix = std::move(p._orbit_model_matrix);
    _sphere_model_matrix = std::move(p._sphere_model_matrix);

    FLOG("Planet move assignment, from %p to %p, texture %u\n", &p, this, _texture);

    return *this;
}

Planet::~Planet() {
    glDeleteTextures(1, &_texture);
}

void Planet::physical_step (float elapsed, bool moving, bool spinning) {
    if (moving) {
        orbitPHI += 2*M_PI * elapsed / siderial_year;
        if (orbitPHI >= 2*M_PI)
            orbitPHI -= 2*M_PI; // Keep it small

        double E = orbitPHI, epsilon = 1e-5;
        if (eccentricity >= 0.8) E = M_PI;
        double fE = E - eccentricity * std::sin(E) - orbitPHI;
        while (std::abs(fE) >= epsilon) {
            E -= fE / (1 - eccentricity * std::cos(E));
            fE = E - eccentricity * std::sin(E) - orbitPHI;
        }

        // '-' for counterclockwise orbiting
        orbitX = semimajor_axis * cos(-E);
        orbitZ = semiminor_axis * sin(-E);
    }

    if (spinning) {
        phase += 360 * elapsed / siderial_day;
        if (phase >= 360.0f)
            phase -= 360.0f;
    }

    for (auto it = moons.begin(); it != moons.end(); it++)
        it->physical_step(elapsed, moving, spinning);
}

mat4 Planet::render(mat4 model_matrix, mat4 mvp_matrix) const {
//    auto crr_model_matrix = model_matrix * _orbit_model_matrix;
    model_matrix *= _orbit_model_matrix;
//    mvp_matrix *= _model_matrix;

//    if (!is_moon) // When rendering moons we are already in planet's orbital plane
////        glRotatef(ECLIPTIC_INCLINATION, 0.0f, 0.0f, 1.0f); // Enter ecliptic plane - common plane of reference
//        mvp_matrix *= glm::rotate(ECLIPTIC_INCLINATION, vec3(0.0f, 0.0f, 1.0f));

    model_matrix *= glm::translate(orbitX, 0.0f, orbitZ);

    auto return_matrix = mvp_matrix * model_matrix;
    for (auto it = moons.begin(); it != moons.end(); it++)
        it->render(model_matrix, mvp_matrix);

//    mvp_matrix = mat4(1.0f);
    model_matrix *= glm::rotate(axis_inclination, 1.0f, 0.0f, 0.0f);
    model_matrix *= glm::rotate(phase, 0.0f, 1.0f, 0.0f);
//    crr_model_matrix *= glm::scale(radius, radius, radius);
    model_matrix *= _sphere_model_matrix;

//    crr_model_matrix*= glm::rotate(90.0f, -1.0f, 0.0f, 0.0f);
    mvp_matrix *= model_matrix;

    _sphere_model.render(model_matrix, mvp_matrix, _texture);

    return return_matrix;
}

mat4 Planet::render_orbit(mat4 matrix) const
{
    matrix *= _orbit_model_matrix;
//    matrix *= glm::rotate(90.0f, 1.0f, 0.0f, 0.f);

    auto scale_matrix = glm::scale(semimajor_axis, 1.0f, semiminor_axis);
//    matrix *= glm::scale(semimajor_axis, 1.0f , semiminor_axis);

    // draw
    _orbit_model.render(glm::mat4(), matrix * scale_matrix, 0);
//    _orbit_model.render(glm::mat4(), matrix , 0);

    // render moons's orbit
//    matrix *= glm::scale(1.0f / semimajor_axis, 1.0f, 1.0f / semiminor_axis);
    matrix *= glm::translate(orbitX, 0.0f, orbitZ);
    for (auto const &moon : moons)
        moon.render_orbit(matrix);

    return matrix;
}
