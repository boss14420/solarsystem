/*
 * =====================================================================================
 *
 *       Filename:  shader.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/11/2013 06:21:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../config.hh"
#include "shader.hh"
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>

#ifndef NDEBUG
    #define FLOG(STR, ...) std::fprintf(stderr, STR, __VA_ARGS__)
#else
    #define FLOG(STR, ...)
#endif

//////////////////////////////////////////////////////////////////////
///// Shader methods /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Shader::Shader(GLenum shader_type, char const *shader_file, char const *prefix_str)
{
    load_file(shader_type, shader_file, prefix_str);    
}


Shader::Shader(Shader &&s)
    : _shader_type(s._shader_type),
      _shaderID(s._shaderID)
{
    s._shaderID = 0;
}


Shader::~Shader() {
    if (glIsShader(_shaderID))
        glDeleteShader(_shaderID);
    _shaderID = 0;
}


GLint Shader::load_file(GLenum shader_type, char const *shader_file, char const *prefix_str)
{
    if (glIsShader(_shaderID))
        glDeleteShader(_shaderID);

    _shader_type = shader_type;

    // Read from file
    std::string shader_code = prefix_str, line;
    shader_code += '\n';
    if (shader_file) {
        std::ifstream ifs(shader_file, std::ios::in);
        if (ifs.fail())
            throw std::fstream::failure(std::string("Couldn't open file ") + shader_file);
        while (std::getline(ifs, line))
            shader_code += line + '\n';
        if (ifs.fail() && !ifs.eof())
            throw std::fstream::failure(std::string("File read error: ") + shader_file);
        ifs.close();
    }

    // Compile shader
    std::fprintf(stdout, "Compiling shader : %s\n", shader_file);
    _shaderID = glCreateShader(shader_type);
    char const *pshader_source = shader_code.data();
    glShaderSource(_shaderID, 1, &pshader_source, nullptr);
    glCompileShader(_shaderID);

    // Check shader
    GLint result = GL_FALSE;
    int InfoLogLength;
    glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> shader_error_message(InfoLogLength + 1);
        glGetShaderInfoLog(_shaderID, InfoLogLength, nullptr, shader_error_message.data());
        std::fputs(shader_error_message.data(), stderr);
        std::fputc('\n', stderr);
    }

    return result;
}

//////////////////////////////////////////////////////////////////////
///// ShaderProgram methods //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

ShaderProgram::ShaderProgram(Shader const &vertex_shader, Shader const &frag_shader)
{
    link(vertex_shader, frag_shader);    
}

ShaderProgram::ShaderProgram(char const *vert_file, char const *frag_file, char const *prefix)
{
    link(vert_file, frag_file, prefix);
}

ShaderProgram::ShaderProgram(ShaderProgram &&sp)
    : _programID(sp._programID)
{
    sp._programID = 0;
}

ShaderProgram::~ShaderProgram() {
    if (glIsProgram(_programID))
        glDeleteProgram(_programID);
    _programID = 0;
}

GLint ShaderProgram::link(Shader const &vertex_shader, Shader const &frag_shader)
{
    if (glIsProgram(_programID))
        glDeleteProgram(_programID);

    // Link the program
    std::fputs("Linking program\n", stdout);
    _programID = glCreateProgram();
    glAttachShader(_programID, vertex_shader._shaderID);
    glAttachShader(_programID, frag_shader._shaderID);
    glLinkProgram(_programID);

    // Check program
    int InfoLogLength;
    GLint result;
    glGetProgramiv(_programID, GL_LINK_STATUS, &result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> program_error_message(InfoLogLength + 1);
        glGetProgramInfoLog(_programID, InfoLogLength, nullptr, program_error_message.data());
        std::fputs(program_error_message.data(), stderr);
        std::fputc('\n', stderr);
    }

    return result;
}

GLint ShaderProgram::link(char const *vert_file, char const *frag_file, char const *prefix)
{
	GL_LOG;
    if (glIsProgram(_programID))
        glDeleteProgram(_programID);
	GL_LOG;
		
    Shader vertex_shader(GL_VERTEX_SHADER, vert_file, prefix);
    Shader frag_shader(GL_FRAGMENT_SHADER, frag_file, prefix);
    return link(vertex_shader, frag_shader);
}

GLint ShaderProgram::get_uniform_location(GLchar const *name) const {
    return glGetUniformLocation(_programID, name);
}

GLint ShaderProgram::get_attrib_location(GLchar const *name) const {
    return glGetAttribLocation(_programID, name);
}

GLenum ShaderProgram::use() const {
    glUseProgram(_programID);
    return glGetError();
}
