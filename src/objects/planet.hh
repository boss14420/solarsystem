
#ifndef __PLANET_HH__
#define __PLANET_HH__

// windows.h must be included before GL headers
#ifdef _MSC_VER
#include <windows.h>
#endif

#include "../config.hh"
#include "../graphics/mesh.hh"
#include "../graphics/circle.hh"
#include "planetdata.hh"

#include <vector>

#include <cstdio>

class SolarSystem;

class Planet;
typedef Planet Star;
typedef Planet Moon;

class Planet {
private:
    Float radius, flattening, semimajor_axis, semiminor_axis;
    Float siderial_year, siderial_day;
    Float orbit_inclination, axis_inclination;
    Float asc_node, arg_periapsis;
    Float orbitX, orbitZ, orbitPHI, phase; // phi in radians, phase in degrees
    std::vector<Moon> moons;

    GLuint _texture;
    mat4 _orbit_model_matrix;
    mat4 _sphere_model_matrix;

    static Mesh _sphere_model;
    static Circle _orbit_model;

public:
    Planet () = default;
    Planet (PlanetData const &pd);

    Planet (Planet const&) = delete;
    Planet (Planet &&planet);

    Planet& operator=(Planet&& p);

    ~Planet();

    friend class SolarSystem;

    void physical_step (float elapsed, bool moving, bool spinning);
    mat4 render (mat4 model_matrix, mat4 mvp_matrix) const;
    mat4 render_orbit (mat4 matrix) const;

//    Planet& add_moon (Planet &&moon); // Use rvalue reference to always steal caller's object - avoids copying OpenGL textures

    template <typename... Args>
    Moon& add_moon (Args&&... args) {
        moons.emplace_back(std::forward<Args>(args)...);
        return moons.back();
    }
};

#endif // __PLANET_HH__
