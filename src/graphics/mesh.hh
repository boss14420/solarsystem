/*
 * =====================================================================================
 *
 *       Filename:  mesh.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2013 12:00:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BOSS14420 (), firefoxlinux at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MESH_HH__
#define __MESH_HH__

#include <glm/glm.hpp>

#include "../config.hh"
#include "shader.hh"
#include "model.hh"
#include <vector>

class Mesh : public Model {
private:
    std::vector<index_type> indices;
    static const std::size_t VERTEX_ATTRIB_SIZE = 3;
    static const std::size_t UV_ATTRIB_SIZE = 2;
    static const std::size_t NORMAL_ATTRIB_SIZE = 3;
    static const std::size_t ATTRIB_SIZE = VERTEX_ATTRIB_SIZE + UV_ATTRIB_SIZE + NORMAL_ATTRIB_SIZE;

    struct attrib {
        FloatData data[ATTRIB_SIZE];
    };
//    struct __attribute__ ((__packed__)) attrib {
//        FloatData indexed_vertex[3];
//        FloatData indexed_uv[2];
//        FloatData indexed_normal[3];
//    };

    std::vector<attrib> attributes;
    static const std::size_t VERTEX_OFFSET = 0;
    static const std::size_t UV_OFFSET = VERTEX_OFFSET + VERTEX_ATTRIB_SIZE*sizeof(FloatData);
    static const std::size_t NORMAL_OFFSET = UV_OFFSET + UV_ATTRIB_SIZE*sizeof(FloatData);

    GLuint element_buffer, attributes_buffer; 
    GLuint vertexID;
    GLuint uvID;
    GLuint normalID;

    GLuint textureID;
    GLuint ModelMatrixID, ViewMatrixID, MatrixID, LightPositionID;

//    GLuint shader_program;

//    mat4 _model_matrix;

public:
    Mesh() = default;
//    Mesh (char const *model_file, char const *vertex_shader, char const *frag_shader);
    Mesh(char const *model_file);

    Mesh(Mesh const &) = delete;
    Mesh (Mesh&& model);

    ~Mesh();

//    GLuint load_texture();

//    mat4 model_matrix() const { return _model_matrix; }
    
//    static void load_model(char const *model_file, char const *vertex_shader, 
//                           char const *frag_shader);
    void use_program(ShaderProgram const &sp);

    void prepare_render(mat4 const &model_matrix, 
                        mat4 const &view_matrix, 
                        mat4 const &mvp_matrix) const;

    template <typename F>
    void set_light_position(F x, F y, F z) const {
        glUniform3f(LightPositionID, x, y, z);
    }

    void render(mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const;

//    void load_model (char const *model_file, char const *vertex_shader, char const *frag_shader);
    bool load_model (char const *model_file);

private:
    void feed_buffers() const;
    void destroy_model();
};

#endif // __MESH_HH__
