//#version 120

#ifdef GL_ES
    precision mediump float;
#endif
#ifdef USE_HALFFLOAT
    // Input vertex data, different for all executions of this shader.
    attribute lowp vec3 vertex_position_modelspace;
    attribute lowp vec2 vertex_uv;
    //attribute lowp vec3 vertex_normal_modelspace;
#else
    attribute vec3 vertex_position_modelspace;
    attribute vec2 vertex_uv;
    //attribute vec3 vertex_normal_modelspace;
#endif

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec3 Position_worldspace;
//varying vec3 Normal_cameraspace;
//varying vec3 EyeDirection_cameraspace;
//varying vec3 LightDirection_cameraspace;
varying vec3 Normal_worldspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform vec3 light_position_worldspace;

void main(){

	// Output position of the vertex, in clip space : mvp_matrix * position
	gl_Position =  mvp_matrix * vec4(vertex_position_modelspace,1);
	
	// Position of the vertex, in worldspace : model_matrix * position
	Position_worldspace = (model_matrix * vec4(vertex_position_modelspace,1)).xyz;
#if 0	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( view_matrix * model_matrix * vec4(vertex_position_modelspace,1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. model_matrix is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( view_matrix * vec4(light_position_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
        // in sphere, normal vector == positon
        vec3 vertex_normal_modelspace = vertex_position_modelspace;

	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( view_matrix * model_matrix * vec4(vertex_normal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
#endif
        vec3 Center_worldspace = (model_matrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
        Normal_worldspace = Position_worldspace - Center_worldspace;
	// UV of the vertex. No special space for this one.
	UV = vertex_uv;
}

// vim: ft=glsl
