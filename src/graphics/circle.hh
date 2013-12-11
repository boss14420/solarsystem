/*
 * =====================================================================================
 *
 *       Filename:  circle.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/11/2013 11:17:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __CIRCLE_HH__
#define __CIRCLE_HH__

#include "../config.hh"
#include "model.hh"
#include "shader.hh"
#include <vector>

class Circle : public Model {
private:
    Float radius;
    std::vector<vec3data> vertices;

    GLuint vertex_buffer, vertexID;

    GLuint MatrixID;

public:
    Circle() = default;
    Circle(std::size_t num_vertices, Float radius);
//    Circle(char const *model_file);
    Circle(Circle const &) = delete;
    Circle(Circle &&);

    ~Circle();

    void use_program(ShaderProgram const &sp);

    void prepare_render(mat4 const &model_matrix, 
                         mat4 const &view_matrix, 
                         mat4 const &mvp_matrix) const;

    void render(mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const;

    void generate(std::size_t num_vertices = 50, Float radius = 1.0);

private:
    void feed_buffers() const;
    void destroy_model();
};

#endif // __CIRCLE_HH__
