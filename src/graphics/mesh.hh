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

//#include <GL/glew.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#include "../config.hh"
#include "shader.hh"
#include "model.hh"
#include <vector>

class Mesh : public Model {
private:
    std::vector<index_type> indices;
    std::vector<vec3data> indexed_vertices;
    std::vector<vec2data> indexed_uvs;
    std::vector<vec3data> indexed_normals;

    GLuint element_buffer;
    GLuint vertex_buffer, vertexID;
    GLuint uv_buffer, uvID;
    GLuint normal_buffer, normalID;

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
//        : _scale_factor(model._scale_factor),
//          _texture(model._texture),
//          _model_matrix(std::move(model._model_matrix))
//    {
//        model._texture = 0;
//    }

    ~Mesh();

//    GLuint load_texture();

//    mat4 model_matrix() const { return _model_matrix; }
    
//    static void load_model(char const *model_file, char const *vertex_shader, 
//                           char const *frag_shader);
    void use_program(ShaderProgram const &sp);

    void prepare_render(mat4 const &model_matrix, 
                         mat4 const &view_matrix, 
                         mat4 const &mvp_matrix) const;

    void render(mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const;

//    void load_model (char const *model_file, char const *vertex_shader, char const *frag_shader);
    bool load_model (char const *model_file);

private:
    void feed_buffers() const;
    void destroy_model();
};

#endif // __MESH_HH__
