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

//#include <GL/glew.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <climits>

#ifdef USE_HALFFLOAT
    #include <glm/gtc/half_float.hpp>
    typedef GLfloat Float;
    typedef glm::half FloatData;
    #define FLOAT_TYPE GL_FLOAT
    #define FLOAT_DATA_TYPE GL_LOW_FLOAT
#else
    #define USE_FLOAT
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

#include <cstdio>
#define GL_LOG std::fprintf(stderr, "%s:%d: GL error code = 0x%x\n", __FILE__, __LINE__, glGetError())

#endif // __CONFIG_HH__
