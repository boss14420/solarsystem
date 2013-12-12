/*
 * =====================================================================================
 *
 *       Filename:  mesh.cc
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
#include "mesh.hh"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "../common/objloader.hpp"
#include "../common/vboindexer.hpp"


//void Mesh::load_model(char const *model_file, char const *vertex_shader, char const *frag_shader)
bool Mesh::load_model(char const *model_file)
{
    destroy_model();

    //////////////////////////////
    //// Load model

    std::vector<vec3data> vertices;
    std::vector<vec2data> uvs;
    std::vector<vec3data> normals; // Won't be used at the moment.
    std::fprintf(stdout, "Loading model : %s\n", model_file);
    bool res = loadOBJ(model_file, vertices, uvs, normals);
    if (!res)
        return false;

    //    std::vector<unsigned short> indices;
    std::vector<vec3data> indexed_vertices;
    std::vector<vec2data> indexed_uvs;
    std::vector<vec3data> indexed_normals;
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    attributes.resize(indexed_vertices.size());
    for(std::size_t i = 0; i != indexed_vertices.size(); ++i) {
        attributes[i] = { indexed_vertices[i][0],indexed_vertices[i][1], indexed_vertices[i][2],
                          indexed_uvs[i][0], indexed_uvs[i][1],
                          indexed_normals[i][0],indexed_normals[i][1],indexed_normals[i][2] };
    }

    ///////////////////////////////
    ///// Bind buffers

    glGenBuffers(1, &attributes_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, attributes_buffer);
    glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(attrib), attributes.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

    return true;
}

void Mesh::use_program(ShaderProgram const &sp)
{
    ///////////////////////////////
    //// Bind attribute && uniform
    vertexID = sp.get_attrib_location("vertex_position_modelspace");
    uvID = sp.get_attrib_location("vertex_uv");
    normalID = sp.get_attrib_location("vertex_normal_modelspace");

    textureID = sp.get_uniform_location("texture");
    ModelMatrixID = sp.get_uniform_location("model_matrix");
    ViewMatrixID = sp.get_uniform_location("view_matrix");
    MatrixID = sp.get_uniform_location("mvp_matrix");
    LightPositionID = sp.get_uniform_location("light_position_worldspace");
}

void Mesh::feed_buffers() const
{
    glBindBuffer(GL_ARRAY_BUFFER, attributes_buffer);

    // 1st attribute : vertices
    glEnableVertexAttribArray(vertexID);
    glVertexAttribPointer(
            vertexID,                           // The attribute we want to configure
            3,                                  // size
            FLOAT_DATA_TYPE,                    // type
            GL_FALSE,                           // normalized?
            sizeof(attrib),                     // stride
            reinterpret_cast<void*>(VERTEX_OFFSET) //  array buffer offset
            );

    // 2nd attribute : UVs
    glEnableVertexAttribArray(uvID);
    glVertexAttribPointer(
            uvID,                               // The attribute we want to configure
            2,                                  // size
            FLOAT_DATA_TYPE,                    // type
            GL_FALSE,                           // normalized?
            sizeof(attrib),                     // stride
            reinterpret_cast<void*>(UV_OFFSET)  //  array buffer offset
            );

    // 3rd attribute : normals
    glEnableVertexAttribArray(normalID);
    glVertexAttribPointer(
            normalID,                           // The attribute we want to configure
            3,                                  // size
            FLOAT_DATA_TYPE,                    // type
            GL_FALSE,                           // normalized?
            sizeof(attrib),                     // stride
            reinterpret_cast<void*>(NORMAL_OFFSET) // array buffer offset
            );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
}

void Mesh::prepare_render (mat4 const &model_matrix, 
                           mat4 const &view_matrix, 
                           mat4 const &mvp_matrix) const
{
//    glUseProgram(shader_program);
    feed_buffers();


    ////////////////////////////////
    /// bind

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view_matrix[0][0]);
}

void Mesh::destroy_model()
{
    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &attributes_buffer);
}


void Mesh::render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const
{
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model_matrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    glDrawElements(GL_TRIANGLES, indices.size(), INDEX_TYPE, (void*) 0);
}

Mesh::Mesh (char const *model_file)
{
    load_model(model_file);
}

Mesh::Mesh (Mesh &&m) :
    indices(std::move(m.indices)), 
    attributes(std::move(m.attributes)),
    element_buffer(m.element_buffer),
    attributes_buffer(m.attributes_buffer),
    vertexID(m.vertexID),
    uvID(m.uvID),
    normalID(m.normalID),
    textureID(m.textureID),
    ModelMatrixID(m.ModelMatrixID),
    ViewMatrixID(m.ViewMatrixID),
    MatrixID(m.MatrixID),
    LightPositionID(m.LightPositionID)
{
    m.element_buffer 
        = m.attributes_buffer 
        = m.vertexID = m.uvID = m.normalID
        = m.textureID
        = m.ModelMatrixID = m.ViewMatrixID
        = m.MatrixID = m.LightPositionID 
        = 0;
}

Mesh::~Mesh()
{
    destroy_model();
}
