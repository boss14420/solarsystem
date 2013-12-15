//#version 120

#ifdef GL_ES
    precision mediump float;
#endif
#ifdef USE_HALFFLOAT
    // Input vertex data, different for all executions of this shader.
    attribute lowp vec2 vertex_position;
    attribute lowp vec2 vertex_uv;
#else
    attribute vec2 vertex_position;
    attribute vec2 vertex_uv;
#endif

// Output data ; will be interpolated for each fragment.
varying vec2 UV;

// Values that stay constant for the whole mesh.
//uniform mat4 mvp_matrix;

void main(){

	// Output position of the vertex, in clip space : mvp_matrix * position
	gl_Position =  vec4(vertex_position, 0.0, 1.0);

        // UV of the vertex. No special space for this one.
        UV = vertex_uv;
}

// vim: ft=glsl
