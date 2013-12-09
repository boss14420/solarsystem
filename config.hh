/*
 * =====================================================================================
 *
 *       Filename:  config.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/09/2013 09:46:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __CONFIG_HH__
#define __CONFIG_HH__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <climits>

#ifdef USE_OPENGLES
#include <glm/gtc/half_float.hpp>
    #define USE_HALFFLOAT

    #define SPHERE_FRAG_SHADER "shaders/sphere.gl.frag"
    #define SPHERE_VERT_SHADER "shaders/sphere.gl.vert"
#else
    #define USE_FLOAT

    #define SPHERE_FRAG_SHADER "shaders/sphere.gl.frag"
    #define SPHERE_VERT_SHADER "shaders/sphere.gl.vert"
#endif

#ifdef USE_HALFFLOAT
    typedef GLfloat Float;
    typedef glm::half FloatData;
    #define FLOAT_TYPE GL_FLOAT
    #define FLOAT_DATA_TYPE GL_HALF_FLOAT
#else
    #define FLOAT_DATA_TYPE GL_FLOAT
    typedef GLfloat Float;
    typedef GLfloat FloatData;
    #define FLOAT_TYPE GL_FLOAT
#endif

typedef glm::detail::tmat4x4<FloatData> mat4data;
typedef glm::detail::tmat4x4<Float> mat4;
typedef glm::detail::tvec2<FloatData> vec2data;
typedef glm::detail::tvec2<Float> vec2;
typedef glm::detail::tvec3<FloatData> vec3data;
typedef glm::detail::tvec3<Float> vec3;
typedef glm::detail::tvec4<FloatData> vec4data;
typedef glm::detail::tvec4<Float> vec4;

typedef std::uint16_t index_type;
#define INDEX_TYPE GL_UNSIGNED_SHORT

#define SPHERE_MODEL "objects/earth.obj"

#endif // __CONFIG_HH__
