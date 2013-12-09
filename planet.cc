
// windows.h must be included before GL headers
#ifdef _MSC_VER
#include <windows.h>
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <GL/glew.h>

#include "constants.h"
#include "planet.hh"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>


#include "common/texture.hpp"


void drawTorus(double, int, int);

Model Planet::_sphere_model;

Planet::Planet(PlanetData const &pd)
  : radius(pd.radius),
    semimajor_axis(pd.semimajor_axis),
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
    _texture(loadDDS(pd.texture_file)),
    _model_matrix(1.0f)
{
    semiminor_axis = semimajor_axis * std::sqrt(1.0f - pd.eccentricity*pd.eccentricity);
//    texture = loadBMPTexture(texture_file);
//    texture = loadBMP_custom(texture_file);

    _model_matrix *= glm::rotate(ECLIPTIC_INCLINATION, 0.0f, 0.0f, 1.0f);

    _model_matrix *= glm::rotate(asc_node, 0.0f, 1.0f, 0.0f);
    _model_matrix *= glm::rotate(orbit_inclination, 0.0f, 0.0f, 1.0f);
    _model_matrix *= glm::rotate(arg_periapsis, 0.0f, 1.0f, 0.0f);

    FLOG("Planet constructor, address %p, texture %u, texture file %s\n", this, _texture, pd.texture_file);
}


Planet::Planet(Float radius_,
               Float semimajor_axis_,
               Float eccentricity,
               Float siderial_year_,
               Float siderial_day_,
               Float orbit_inclination_,
               Float axis_inclination_,
               Float asc_node_,
               Float arg_periapsis_,
               const char *texture_file,
               Float phi) :
    radius(radius_),
    semimajor_axis(semimajor_axis_),
    siderial_year(siderial_year_),
    siderial_day(siderial_day_),
    orbit_inclination(orbit_inclination_),
    axis_inclination(axis_inclination_),
    asc_node(asc_node_),
    arg_periapsis(arg_periapsis_),
    orbitX(0.0f),
    orbitZ(0.0f),
    orbitPHI(phi),
    phase(0.0f),
    _texture(loadBMP_custom(texture_file)),
    _model_matrix(1.0f)
{
    semiminor_axis = semimajor_axis * std::sqrt(1.0f - eccentricity*eccentricity);
//    texture = loadBMPTexture(texture_file);
//    texture = loadBMP_custom(texture_file);

    _model_matrix *= glm::rotate(ECLIPTIC_INCLINATION, 0.0f, 0.0f, 1.0f);

    _model_matrix *= glm::rotate(asc_node, 0.0f, 1.0f, 0.0f);
    _model_matrix *= glm::rotate(orbit_inclination, 0.0f, 0.0f, 1.0f);
    _model_matrix *= glm::rotate(arg_periapsis, 0.0f, 1.0f, 0.0f);

    FLOG("Planet constructor, address %p, texture %u, texture file %s\n", this, _texture, texture_file);
}

Planet::Planet(Planet &&rvalue) :
    radius(rvalue.radius),
    semimajor_axis(rvalue.semimajor_axis),
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
    moons(std::move(rvalue.moons))
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
    semiminor_axis = p.semimajor_axis;
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

    FLOG("Planet move assignment, from %p to %p, texture %u\n", &p, this, _texture);

    return *this;
}

Planet::~Planet() {
    glDeleteTextures(1, &_texture);
}

void Planet::move (int elapsed) {
    orbitPHI += DAYS_PER_SECOND * 2*M_PI * elapsed / (1000 * siderial_year);
    if (orbitPHI >= 2*M_PI)
        orbitPHI -= 2*M_PI; // Keep it small

    // '-' for counterclockwise orbiting
    orbitX = semimajor_axis * cos(-orbitPHI);
    orbitZ = semiminor_axis * sin(-orbitPHI);

    phase += 360 * DAYS_PER_SECOND * elapsed / (1000 * siderial_day);
    if (phase >= 360.0f)
        phase -= 360.0f;

    for (auto it = moons.begin(); it != moons.end(); it++)
        it->move(elapsed);
}

mat4 Planet::render(mat4 matrix) const {
//    glPushMatrix();
//
//    glColor3f(1.0f, 1.0f, 1.0f);

    auto crr_model_matrix = _model_matrix;
//    matrix *= _model_matrix;

//    if (!is_moon) // When rendering moons we are already in planet's orbital plane
////        glRotatef(ECLIPTIC_INCLINATION, 0.0f, 0.0f, 1.0f); // Enter ecliptic plane - common plane of reference
//        matrix *= glm::rotate(ECLIPTIC_INCLINATION, vec3(0.0f, 0.0f, 1.0f));
//
////    glRotatef(asc_node, 0.0f, 1.0f, 0.0f); // Place orbital nodes
//////    glRotatef(orbit_inclination, 0.0f, 0.0f, 1.0f); // Now handle inclination
////    glRotatef(arg_periapsis, 0.0f, 1.0f, 0.0f); // And finally argument of periapsis
//
//    model_matrix *= glm::rotate(asc_node, 0.0f, 1.0f, 0.0f);
//    model_matrix *= glm::rotate(orbit_inclination, 0.0f, 0.0f, 1.0f); // Now handle inclination
//    model_matrix *= glm::rotate(arg_periapsis, 0.0f, 1.0f, 0.0f); // And finally argument of periapsis
//
//    if (orbit) {
//        glPushMatrix();
//        mat4 orbit_matrix(1.0);
////        glRotatef(90.f, 1.0f, 0.0f, 0.0f);
////        glScalef(1.0f, semiminor_axis / semimajor_axis, 1.0f);
////        drawTorus(semimajor_axis, 5, 100);
//
//        orbit_matrix *= glm::rotate(90.f, 1.0f, 0.0f, 0.f);
//        orbit_matrix *= glm::scale(1.0f, semiminor_axis / semimajor_axis, 1.0f);
//        glMultMatrixf(&orbit_matrix[0][0]);
//        drawTorus(semimajor_axis, 5, 100);
//
//        glPopMatrix();
//    }

    /*
    glBegin(GL_LINES);
    glVertex3f(-semimajor_axis, 0.0f, 0.0f);
    glVertex3f(semimajor_axis, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -semiminor_axis);
    glVertex3f(0.0f, 0.0f, semiminor_axis);
    glEnd();
    */

//    glTranslatef(orbitX, 0.0f, orbitZ);
    crr_model_matrix *= glm::translate(orbitX, 0.0f, orbitZ);
//    glMultMatrixf(&model_matrix[0][0]);

    for (auto it = moons.begin(); it != moons.end(); it++)
        it->render(matrix * crr_model_matrix);

//    glLoadIdentity();

//    glRotatef(axis_inclination, 1.0f, 0.0f, 0.0f); // Axis is inclined wrt orbit
//    glRotatef(phase, 0.0f, 1.0f, 0.0f); // Finally handle everyday rotation
//
//    glRotatef(90.0f, -1.0f, 0.0f, 0.0f); // Rotate a bit so texture is applied correctly

//    matrix = mat4(1.0f);
    crr_model_matrix *= glm::rotate(axis_inclination, 1.0f, 0.0f, 0.0f);
    crr_model_matrix *= glm::rotate(phase, 0.0f, 1.0f, 0.0f);
    crr_model_matrix *= glm::scale(radius, radius, radius);

//    crr_model_matrix*= glm::rotate(90.0f, -1.0f, 0.0f, 0.0f);
    matrix *= crr_model_matrix;

//    glMultMatrixf(&model_matrix[0][0]);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
//    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
//    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &crr_model_matrix[0][0]);

//    std::cerr << "Model matrix: " << glm::to_string(crr_model_matrix)
//              << "\nMVP matrix: " << glm::to_string(matrix) << "\n\n";

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, _texture);
//    glUniform1i(textureID, 0);
//
//    glDrawElements(GL_TRIANGLES, indices.size(), INDEX_TYPE, (void*) 0);

    _sphere_model.render(crr_model_matrix, matrix, _texture);

    //    GLUquadricObj *planet = gluNewQuadric();
//    gluQuadricDrawStyle(planet, GLU_FILL);
//    gluQuadricTexture(planet, GLU_TRUE);
//    gluQuadricNormals(planet, GLU_SMOOTH);
//
//    gluSphere(planet, radius, 50, 50);
//
//    gluDeleteQuadric(planet);
//    glBindTexture(GL_TEXTURE_2D, 0);


    return matrix;
}

mat4 Planet::render_orbit(mat4 matrix) const
{
    matrix *= _model_matrix;
    matrix *= glm::rotate(90.0f, 1.0f, 0.0f, 0.f);
    matrix *= glm::scale(1.0f, semiminor_axis / semimajor_axis, 1.0f);

    // draw

    return matrix;
}

//void Planet::addMoon(Planet &&moon) {
//    moons.push_back(std::move(moon));
//}

// Taken from google, claimed to be from Red Book
void drawTorus(double r, int numc, int numt) {
    int i, j, k;
    double s, t, x, y, z, twopi;

    twopi = 2 * (double) M_PI;
    for (i = 0; i < numc; i++) {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc + 0.5;
                t = j % numt;

                x = (r + .001*cos(s*twopi/numc))*cos(t*twopi/numt);
                y = (r + .001*cos(s*twopi/numc))*sin(t*twopi/numt);
                z = .001 * sin(s * twopi / numc);
                glVertex3f(x, y, z);
            }
        }
    glEnd();
    }
}
