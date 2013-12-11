
#ifndef __STAR_HH__
#define __STAR_HH__

// windows.h must be included before GL headers
#ifdef _MSC_VER
#include <windows.h>
#endif

#include "config.hh"
#include "model.hh"
#include "planetdata.hh"
#include "planet.hh"

#include <vector>

#include <cstdio>
#define FLOG(STR, ...) std::fprintf(stderr, STR, __VA_ARGS__)

class SolarSystem;

class Star : public Planet {
private:
    static Model _sphere_model;

//    Float radius, semimajor_axis, semiminor_axis;
//    Float siderial_year, siderial_day;
//    Float orbit_inclination, axis_inclination;
//    Float asc_node, arg_periapsis;
//    Float orbitX, orbitZ, orbitPHI, phase; // phi in radians, phase in degrees
//    std::vector<Planet> planets;
//
//    GLuint _texture;
//    mat4 _model_matrix;

public:
    Star () = default;
    Star (PlanetData const &pd);
//    Star (Float radius_,
//            Float semimajor_axis_,
//            Float eccentricity,
//            Float siderial_year_,
//            Float siderial_day_,
//            Float orbit_inclination_,
//            Float axis_inclination_,
//            Float asc_node_,
//            Float arg_periapsis_,
//            const char *texture_file,
//            Float phi = 0.0f);

    Star (Star const&) = delete;
    Star (Star &&star);

    Star& operator=(Star&& p);

//    ~Star();

    friend class SolarSystem;

//    void move (int elapsed);
//    mat4 render (mat4 matrix) const;
//    mat4 render_orbit (mat4 matrix) const;
//
////    Star& add_moon (Star &&moon); // Use rvalue reference to always steal caller's object - avoids copying OpenGL textures

//    template <typename... Args>
//    Planet& add_planet (Args&&... args) {
//        planets.emplace_back(std::forward<Args>(args)...);
//        return planets.back();
//    }
};

#endif // __STAR_HH__
