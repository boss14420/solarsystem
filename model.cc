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
#include "model.hh"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/shader.hpp"


////////////////////////////////////////////////////////////////////
/////// STATIC METHODS /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


void Model::load_model(char const *model_file, char const *vertex_shader, char const *frag_shader)
{
    //////////////////////////////
    //// Load model

    std::vector<vec3data> vertices;
    std::vector<vec2data> uvs;
    std::vector<vec3data> normals; // Won't be used at the moment.
    bool res = loadOBJ(model_file, vertices, uvs, normals);
    
    (void*) res;

    //    std::vector<unsigned short> indices;
    //    std::vector<vec3> indexed_vertices;
    //    std::vector<vec2> indexed_uvs;
    //    std::vector<vec3> indexed_normals;
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), indexed_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), indexed_normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), indexed_uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


    ///////////////////////////////
    ///// Load shaders
    shader_program = LoadShaders(vertex_shader, frag_shader);

    ///////////////////////////////
    //// Bind attribute && uniform
    vertexID = glGetAttribLocation(shader_program, "vertex_position_modelspace");
    uvID = glGetAttribLocation(shader_program, "vertex_uv");
    normalID = glGetAttribLocation(shader_program, "vertex_normal_modelspace");

    textureID = glGetUniformLocation(shader_program, "texture");
    ModelMatrixID = glGetUniformLocation(shader_program, "model_matrix");
    ViewMatrixID = glGetUniformLocation(shader_program, "view_matrix");
    MatrixID = glGetUniformLocation(shader_program, "mvp_matrix");
    LightPositionID = glGetUniformLocation(shader_program, "light_position_worldspace");
}


void Model::feed_buffers()
{
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
            vertexID,                     // The attribute we want to configure
            3,                            // size
            FLOAT_DATA_TYPE,              // type
            GL_FALSE,                     // normalized?
            0,                            // stride
            (void*)0                      // array buffer offset
            );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(uvID);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(
            uvID,                         // The attribute we want to configure
            2,                            // size : U+V => 2
            FLOAT_DATA_TYPE,              // type
            GL_FALSE,                     // normalized?
            0,                            // stride
            (void*)0                      // array buffer offset
            );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(normalID);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(
            normalID,    // The attribute we want to configure
            3,                            // size
            FLOAT_DATA_TYPE,              // type
            GL_FALSE,                     // normalized?
            0,                            // stride
            (void*)0                      // array buffer offset
            );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
}

void Model::prepare_render (mat4 const &model_matrix, 
                            mat4 const &view_matrix, 
                            mat4 const &mvp_matrix)
{
    glUseProgram(shader_program);
    feed_buffers();


    ////////////////////////////////
    /// bind

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view_matrix[0][0]);
    glUniform3f(LightPositionID, 0.0f, 0.0f, 0.0f);
    (void*) &model_matrix;
}

void Model::destroy_model()
{
    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &uv_buffer);
    glDeleteBuffers(1, &normal_buffer);
}


void Model::render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const
{
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model_matrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    glDrawElements(GL_TRIANGLES, indices.size(), INDEX_TYPE, (void*) 0);
}

//////////////////////////////////////////////////////////////////////
//////                         ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Model::Model (char const *model_file, char const *vertex_shader, char const *frag_shader) 
{
    load_model(model_file, vertex_shader, frag_shader);
}

Model::~Model()
{
//    glDeleteTextures(1, &_texture);
    destroy_model();
}