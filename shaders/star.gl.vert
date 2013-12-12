//#version 120

#ifdef GL_ES
    precision mediump float;
#endif
#ifdef USE_HALFFLOAT
    // Input vertex data, different for all executions of this shader.
    attribute lowp vec3 vertex_position_modelspace;
    attribute lowp vec2 vertex_uv;
#else
    attribute vec3 vertex_position_modelspace;
    attribute vec2 vertex_uv;
#endif

// Output data ; will be interpolated for each fragment.
varying vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 mvp_matrix;

void main(){

	// Output position of the vertex, in clip space : mvp_matrix * position
	gl_Position =  mvp_matrix * vec4(vertex_position_modelspace,1);

        // UV of the vertex. No special space for this one.
	UV = vertex_uv;
}

// vim: ft=glsl
