//#version 120

// Interpolated values from the vertex shaders
#ifdef GL_ES
    precision mediump float;
#endif

varying vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D texture;

void main(){
        gl_FragColor = vec4(texture2D(texture, UV).rgb, 1.0);
}

// vim: ft=glsl
