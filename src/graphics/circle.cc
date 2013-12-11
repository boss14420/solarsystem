/*
 * =====================================================================================
 *
 *       Filename:  circle.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/11/2013 11:20:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <cstdio>
#include <deque>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "circle.hh"

////
Circle::Circle(std::size_t num_vertices, Float radius)
    : radius(radius)
{
    generate(num_vertices, radius);
}

////
Circle::Circle(Circle &&c) :
    radius(c.radius),
    vertices(std::move(c.vertices)),
    vertex_buffer(c.vertex_buffer),
    vertexID(c.vertexID),
    MatrixID(c.MatrixID)
{
    c.vertex_buffer = c.vertexID = c.MatrixID = 0;
}

////
Circle::~Circle()
{
    glDeleteBuffers(1, &vertex_buffer);
}

////
void Circle::generate(std::size_t num_vertices, Float radius)
{
    glDeleteBuffers(1, &vertex_buffer);
    this->radius = radius;

    /////////////
    // Generate vertices array
    num_vertices /= 4;
    vertices.resize(num_vertices); 
    vertices[0] = vec3data(0, 0, radius);
    float dphi = M_PI / 2/ (num_vertices - 1), phi, x, z;
    for (std::size_t i = 1; i != num_vertices; ++i) {
        phi = i * dphi;
        z = radius * std::cos(phi);
        x = radius * std::sin(phi);
        vertices[i] = vec3data(x, 0, z);
    }

    //////////
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3data),
                 vertices.data(), GL_STATIC_DRAW);
}

////
void Circle::use_program(ShaderProgram const &sp)
{
    vertexID = sp.get_attrib_location("vertex_position_modelspace"); 
    MatrixID = sp.get_uniform_location("mvp_matrix");
}

////
void Circle::feed_buffers() const
{
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
}

////
void Circle::prepare_render (mat4 const &model_matrix, 
                            mat4 const &view_matrix, 
                            mat4 const &mvp_matrix) const
{
//    glUseProgram(shader_program);
    feed_buffers();


    ////////////////////////////////
    /// bind

//    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
}

////
void Circle::render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const
{
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp_matrix[0][0]);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

    auto mvp2 = glm::scale(mvp_matrix, glm::vec3(1, 1, -1));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

    mvp2 = glm::scale(mvp_matrix, glm::vec3(-1, 1, 1));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

    mvp2 = glm::scale(mvp_matrix, glm::vec3(-1, 1, -1));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}
