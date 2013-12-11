/*
 * =====================================================================================
 *
 *       Filename:  model.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2013 12:08:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <vector>
#include <cstdio>
#include "model.hh"

bool Model::load_model(char const *model_file)
{
    return true;
}

void Model::use_program(ShaderProgram const &sp)
{
}

void Model::prepare_render (mat4 const &model_matrix, 
                            mat4 const &view_matrix, 
                            mat4 const &mvp_matrix)
{
}

void Model::render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const
{
}
