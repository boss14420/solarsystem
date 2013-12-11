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

//#include <GL/glew.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#include "../config.hh"
#include "shader.hh"
#include <vector>

class Model {
protected:
public:
    Model () = default;

    Model (Model const &) = delete;

//    ~Model();

//    GLuint load_texture();

//    mat4 model_matrix() const { return _model_matrix; }
    
//    static void load_model(char const *model_file, char const *vertex_shader, 
//                           char const *frag_shader);
    void use_program(ShaderProgram const &sp);

    void prepare_render (mat4 const &model_matrix, 
                         mat4 const &view_matrix, 
                         mat4 const &mvp_matrix);

    void render (mat4 const &model_matrix, mat4 const &mvp_matrix, GLuint texture) const;

//    void load_model (char const *model_file, char const *vertex_shader, char const *frag_shader);
    bool load_model (char const *model_file);

private:
//    void feed_buffers ();
//    void destroy_model ();
};

#endif // __MODEL_HH__
