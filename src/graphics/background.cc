/*
 * =====================================================================================
 *
 *       Filename:  background.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2013 05:48:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "background.hh"
#include "../common/texture.hpp"


const FloatData Background::vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

const FloatData Background::uvs[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
};



#define BACKGROUND_VERT_SHADER "shaders/background.gl.vert"
#define BACKGROUND_FRAG_SHADER "shaders/background.gl.frag"

#ifdef USE_HALFFLOAT
    #define DEFINITION "#define USE_HALFFLOAT"
#else
    #define DEFINITION ""
#endif
Background::Background(char const *texture_file)
    : _texture(loadTexture(texture_file)),  
      _shader_program(BACKGROUND_VERT_SHADER, BACKGROUND_FRAG_SHADER, DEFINITION)
{
    _textureID = _shader_program.get_uniform_location("texture");
    _vertexID = _shader_program.get_attrib_location("vertex_position");
    _uvID = _shader_program.get_attrib_location("vertex_uv");

    _texture = loadTexture("textures/starmap");

    
    glGenBuffers(1, &_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), uvs, GL_STATIC_DRAW);
}
#undef DEFINITION

Background::~Background() {
    glDeleteTextures(1, &_textureID);
}

void Background::render() const {
    _shader_program.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(_textureID, 0);

    glEnableVertexAttribArray(_vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    glVertexAttribPointer(_vertexID, 2, FLOAT_DATA_TYPE, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(_uvID);
    glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
    glVertexAttribPointer(_uvID, 2, FLOAT_DATA_TYPE, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

