/*
 * =====================================================================================
 *
 *       Filename:  model.hh
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

#ifndef __MODEL_HH__
#define __MODEL_HH__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "config.hh"
#include <vector>

class Model {
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

    GLuint shader_program;

//    mat4 _model_matrix;

public:
    Model () = default;
    Model (char const *model_file, char const *vertex_shader, char const *frag_shader);

    Model (Model const &) = delete;
//    Model (Model&& model) 
//        : _scale_factor(model._scale_factor),
//          _texture(model._texture),
//          _model_matrix(std::move(model._model_matrix))
//    {
//        model._texture = 0;
//    }

    ~Model();

//    GLuint load_texture();

//    mat4 model_matrix() const { return _model_matrix; }
    
//    static void load_model(char const *model_file, char const *vertex_shader, 
//                           char const *frag_shader);
    void prepare_render (mat4 const &model_matrix, 
                         mat4 const &view_matrix, 
                         mat4 const &mvp_matrix);

    void render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const;

    void load_model (char const *model_file, char const *vertex_shader, char const *frag_shader);

private:
    void feed_buffers ();
    void destroy_model ();
};

#endif // __MODEL_HH__
