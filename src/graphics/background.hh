/*
 * =====================================================================================
 *
 *       Filename:  background.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2013 05:41:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __BACKGROUND_HH__
#define __BACKGROUND_HH__

#include "../config.hh"
#include "shader.hh"

class Background {
private:
    GLuint _textureID, _vertexID, _uvID;
    GLuint _texture, _vertexbuffer, _uvbuffer;
    ShaderProgram _shader_program;

    static const FloatData vertices[];
    static const FloatData uvs[];

public:
    Background() = default;
    Background(char const *texture_file);
    Background(Background const&) = delete;
    ~Background();

    void render() const;
};

#endif // __BACKGROUND_HH__
