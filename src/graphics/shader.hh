/*
 * =====================================================================================
 *
 *       Filename:  shader.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/11/2013 06:12:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __SHADER_HH__
#define __SHADER_HH__

#include <GLES2/gl2.h>

class ShaderProgram;

class Shader {
private:
    GLenum _shader_type;
    GLuint _shaderID;

public:
    Shader() = default;
    Shader(Shader &&s);
    Shader(Shader const &s) = delete;
    Shader(GLenum shader_type, char const *shader_file, char const *prefix_str);
    ~Shader();

    friend class ShaderProgram;
};

class ShaderProgram {
private:
    GLuint _programID;
//    Shader const *_vertex_shader, const *_frag_shader;

public:
    ShaderProgram() = default;
    ShaderProgram(ShaderProgram const &sp) = delete;
    ShaderProgram(ShaderProgram &&sp);
    ShaderProgram(char const *vert, char const *frag, char const *prefix = nullptr);
    ShaderProgram(Shader const &vertex_shader, Shader const &frag_shader);
    ~ShaderProgram();

    GLint link(Shader const &vertex_shader, Shader const &frag_shader);

    GLint get_uniform_location(GLchar const *name) const;
    GLint get_attrib_location(GLchar const *name) const;
    GLenum use() const;
};

#endif // __SHADER_HH__
