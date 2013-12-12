//#version 120

#ifdef GL_ES
    precision mediump float;
#endif
#ifdef USE_HALFFLOAT
    // Input vertex data, different for all executions of this shader.
    attribute lowp vec3 vertex_position_modelspace;
#else
    attribute vec3 vertex_position_modelspace;
#endif

// Values that stay constant for the whole mesh.
uniform mat4 mvp_matrix;

void main(){
    // Output position of the vertex, in clip space : mvp_matrix * position
    gl_Position =  mvp_matrix * vec4(vertex_position_modelspace,1);
}

// vim: ft=glsl
