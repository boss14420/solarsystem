//#version 120

// Interpolated values from the vertex shaders
#ifdef GL_ES
    precision mediump float;
#endif

varying vec2 UV;
varying vec3 Position_worldspace;
//varying vec3 Normal_cameraspace;
//varying vec3 EyeDirection_cameraspace;
//varying vec3 LightDirection_cameraspace;
varying vec3 Normal_worldspace;

// Values that stay constant for the whole mesh.
uniform sampler2D texture;
//uniform mat4 MV;
uniform vec3 light_position_worldspace;

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0;
	
	// Material properties
	vec3 MaterialDiffuseColor = texture2D( texture, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.07,0.07,0.07) * MaterialDiffuseColor;
	//vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
#if 0
	// Distance to the light
	float distance = length( light_position_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0.0, 1.0 );
	
	// Eye vector (towards the camera)
	//vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	//vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	//float cosAlpha = clamp( dot( E,R ), 0.0, 1.0 );
#endif
        
        vec3 LightDirection_worldspace = light_position_worldspace - Position_worldspace;
        float distance = length(LightDirection_worldspace);
        vec3 n = normalize(Normal_worldspace);
        vec3 l = normalize(LightDirection_worldspace);
        float cosTheta = clamp(dot(n, l), 0.0, 1.0);
        
	gl_FragColor.rgb = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance);// +
		// Specular : reflective highlight, like a mirror
		//MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
        gl_FragColor.a = 1.0;

        //gl_FragColor = vec4(MaterialDiffuseColor, 1.0);

}

// vim: ft=glsl
